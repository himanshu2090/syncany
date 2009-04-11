#include "SyncAny.h"

/*
 * 已经接收的缓存中是否存在一个完整的行
 */
PM_BOOL SocketIO::hasLine()
{
	int iLen = m_input_buffer->length();
  	if (iLen > 0)
	{
		char* psz = m_input_buffer->rd_ptr();
		for (int i = 0 ; i < iLen ; ++i)
		{
			if (psz[i] == '\n')
			{
				return PM_TRUE;
			}
		}
	}
	return PM_FALSE;
}


/**
 * 从socket stream中读取数据。
 * return : 0   recevie FIN 
 *          >0  recevie data length
 *          <0  失败
 */
int SocketIO::recv() 
{
	{
		if (m_iremoteport == 0)
		{
			ACE_INET_Addr RemoteAdd;
			m_pAceStream->get_remote_addr(RemoteAdd);
			m_iremoteport = RemoteAdd.get_port_number();
		}
	}

	int space = m_input_buffer->space();
    if ( space > 0 ) 
    {
        ACE_Time_Value timeout(0,0);
        char* wr_ptr = m_input_buffer->wr_ptr();
		errno = 0;
        int n = m_pAceStream->recv(wr_ptr,space,&timeout);//立即返回不需要考虑慢调用被打断的情况
        if ( n == 0 ) 
        {
            return 0;
        }
        else if ( n > 0 )
        {
            m_input_buffer->wr_ptr(n);
            return n;
        }
        else 
        {
			return -1;
        }
    }
	return 0;
}

/**
 * 从缓冲区中读取命令，读取完整的一行命令。
 * 
 * string          保存读取的命令。
 * ACE_Time_Value  不用。
 * return : 0   recevie FIN 
 *          >0  recevie data length
 *          -1  没有读到完整的一行命令。
 */
int SocketIO::get_line(string& line,ACE_Time_Value* time) 
{
	int ret;
	char ch;
    char* rd_ptr = m_input_buffer->rd_ptr();
    int length = m_input_buffer->length();
	while(length > 0 )
	{
        ch = *rd_ptr;
        
        if ( ch == '\r' ) {++rd_ptr;--length;continue;}
		if( ch == '\n' )
		{
            ++rd_ptr;
			break;
		}
		line.push_back(ch);
	}
    
    if ( ch == '\n' ) 
    {
        //读到换行
        m_input_buffer->rd_ptr(rd_ptr);
        //如果缓冲区内已经没有数据，重置读指针。
        if ( m_input_buffer->length() == 0 ) {m_input_buffer->reset();}
    }
    else 
    {
        //没有读到换行
        line = "";
        return -1;
    }    

	return line.size();
}



int SocketIO::get_line(PmString& line,ACE_Time_Value* time) 
{
    char* rd_ptr = m_input_buffer->rd_ptr();
    int length = m_input_buffer->length();
	int i = 0;

	while(i < length && rd_ptr[i] != '\n' && ++i)
	{
	}

	if (i == length)
	{
		//没有读取到换行符
		line = "";
		return -1;
	}
	else
	{
		bytestr_copy_data(line,(PM_BYTE*)(rd_ptr),i + 1);
		m_input_buffer->rd_ptr(i+1);
        //如果缓冲区内已经没有数据，重置读指针。
        if ( m_input_buffer->length() == 0 ) 
		{
			m_input_buffer->reset();
		}
		return i+1;
    }
}

//将数据写入到缓存中
int SocketIO::write_to_cache(const PmString& strOutPut)
{
	int nLen = strOutPut.length();
	if (nLen > 0)
	{
		char* wr_ptr = get_wr_ptr();
		memcpy(wr_ptr,strOutPut.c_str(),nLen);
		set_wr_ptr(nLen);
	}
	return 0;
}



/**
 * 发送缓冲区内的数据。
 *  
 * int&           返回缓冲区剩余长度。
 * ACE_Time_Value 
 *
 * return         已经发送的数据。
 */
int SocketIO::write_out(int& s,ACE_Time_Value*) 
{
    int len = length();
    ACE_Time_Value timeout(0,0);
    char* r_ptr = get_rd_ptr();
    int n = m_pAceStream->send(r_ptr,len,&timeout);
    if ( n > 0 ) 
    {
		set_rd_ptr(n);
	}

	if ( len == n ) 
	{
		reset();
	}

	return length();
}





//将缓冲区中所有的数据全部写出
int SocketIO::write_all_out(ACE_Time_Value* pTime) 
{
	int len = length();
	char* r_ptr = get_rd_ptr();
	int n = m_pAceStream->send_n(r_ptr,len);

	if (n > 0)
	{
		//重置读指针
		pm_debug2("send count<%d>,port<%d>",n,m_iremoteport);
		set_rd_ptr(n);
	}
	if (len == n)
	{
		//输出缓冲区内的数据都已发送出去，重置输出缓冲区读写起始地址。
		reset();
	}
	return length();
}



/*
 * 读取输入缓冲区内的所有数据，并添加到strBuf的后面
 * strBuf是一个输入输出型的参数
 */
int SocketIO::read_all_append(PmString& strBuf,ACE_Time_Value* pTime) 
{
	char* rd_ptr = m_input_buffer->rd_ptr();
    int iLen= m_input_buffer->length();
	if (iLen == 0)
	{
		return 0;
	}

	bytestr_strncat(strBuf,(PM_BYTE*)rd_ptr,iLen);
	m_input_buffer->rd_ptr(iLen);
	//如果缓冲区内已经没有数据，重置读指针。
    if ( m_input_buffer->length() == 0) 
	{
		m_input_buffer->reset();
	}
	return iLen;
}


/**
 * 发送数据。
 * 直接将传入的数据发送出去，没有发送出去的数据则写入输出缓冲区。
 * 内部实现时，首先发送缓冲区内的数据，仅当缓冲区内的数据发送完成后，
 * 才发送此数据。
 * 
 * char*           需要发送的数据。
 * int             需要发送的数据长度。
 * int&            返回输出缓冲区的剩余空间长度。
 * ACE_Time_Value  不用。
 * return          已发送和已缓存的数据长度和。
 */
int SocketIO::send (char* buf,int len,int& s,ACE_Time_Value*) 
{
    char* tmpbuf = buf;
    int   tmplen = len;
    int n = write_out(s,PM_NULL);
    int s_len = length();
    if ( s_len == 0 ) 
    {
        //缓冲区内的数据已经发送完成。
        ACE_Time_Value timeout(0,0);
        n = m_pAceStream->send(buf,len,&timeout);
        if ( n >0 ) 
        {
            tmpbuf += n;
            tmplen -= n;
        }
    }
    s = space();
    char* wr_ptr = get_wr_ptr();
    memcpy(wr_ptr,tmpbuf,s>tmplen?tmplen:s);
    //set_wr_ptr(wr_ptr+(s>tmplen?tmplen:s));
    set_wr_ptr(s>tmplen?tmplen:s);
    tmplen -= s;
    
    //输出缓冲区剩余空间
    s = space();

    return tmplen>0?(len-tmplen):len;
}

/**
 * 取得，设置输出缓冲区写起始地址。
 */
char* SocketIO::get_wr_ptr(){return m_output_buffer->wr_ptr();}
void  SocketIO::set_wr_ptr(int nBytes) {m_output_buffer->wr_ptr(nBytes);}

/**
 * 取得，设置输出缓冲区读起始地址。
 */
char* SocketIO::get_rd_ptr(){return m_output_buffer->rd_ptr();}
void  SocketIO::set_rd_ptr(int nBytes){m_output_buffer->rd_ptr(nBytes);}




/**
 * 取得输出缓冲区剩余空间大小。
 */
int SocketIO::space() {return m_output_buffer->space();}

/**
 * 取得输出缓冲区数据长度。
 */
int SocketIO::length(){return m_output_buffer->length();}

//重置输出缓冲区
void SocketIO::reset() {m_output_buffer->reset();}





