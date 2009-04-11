#include "SyncAny.h"

/*
 * �Ѿ����յĻ������Ƿ����һ����������
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
 * ��socket stream�ж�ȡ���ݡ�
 * return : 0   recevie FIN 
 *          >0  recevie data length
 *          <0  ʧ��
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
        int n = m_pAceStream->recv(wr_ptr,space,&timeout);//�������ز���Ҫ���������ñ���ϵ����
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
 * �ӻ������ж�ȡ�����ȡ������һ�����
 * 
 * string          �����ȡ�����
 * ACE_Time_Value  ���á�
 * return : 0   recevie FIN 
 *          >0  recevie data length
 *          -1  û�ж���������һ�����
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
        //��������
        m_input_buffer->rd_ptr(rd_ptr);
        //������������Ѿ�û�����ݣ����ö�ָ�롣
        if ( m_input_buffer->length() == 0 ) {m_input_buffer->reset();}
    }
    else 
    {
        //û�ж�������
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
		//û�ж�ȡ�����з�
		line = "";
		return -1;
	}
	else
	{
		bytestr_copy_data(line,(PM_BYTE*)(rd_ptr),i + 1);
		m_input_buffer->rd_ptr(i+1);
        //������������Ѿ�û�����ݣ����ö�ָ�롣
        if ( m_input_buffer->length() == 0 ) 
		{
			m_input_buffer->reset();
		}
		return i+1;
    }
}

//������д�뵽������
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
 * ���ͻ������ڵ����ݡ�
 *  
 * int&           ���ػ�����ʣ�೤�ȡ�
 * ACE_Time_Value 
 *
 * return         �Ѿ����͵����ݡ�
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





//�������������е�����ȫ��д��
int SocketIO::write_all_out(ACE_Time_Value* pTime) 
{
	int len = length();
	char* r_ptr = get_rd_ptr();
	int n = m_pAceStream->send_n(r_ptr,len);

	if (n > 0)
	{
		//���ö�ָ��
		pm_debug2("send count<%d>,port<%d>",n,m_iremoteport);
		set_rd_ptr(n);
	}
	if (len == n)
	{
		//����������ڵ����ݶ��ѷ��ͳ�ȥ�����������������д��ʼ��ַ��
		reset();
	}
	return length();
}



/*
 * ��ȡ���뻺�����ڵ��������ݣ�����ӵ�strBuf�ĺ���
 * strBuf��һ����������͵Ĳ���
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
	//������������Ѿ�û�����ݣ����ö�ָ�롣
    if ( m_input_buffer->length() == 0) 
	{
		m_input_buffer->reset();
	}
	return iLen;
}


/**
 * �������ݡ�
 * ֱ�ӽ���������ݷ��ͳ�ȥ��û�з��ͳ�ȥ��������д�������������
 * �ڲ�ʵ��ʱ�����ȷ��ͻ������ڵ����ݣ������������ڵ����ݷ�����ɺ�
 * �ŷ��ʹ����ݡ�
 * 
 * char*           ��Ҫ���͵����ݡ�
 * int             ��Ҫ���͵����ݳ��ȡ�
 * int&            ���������������ʣ��ռ䳤�ȡ�
 * ACE_Time_Value  ���á�
 * return          �ѷ��ͺ��ѻ�������ݳ��Ⱥ͡�
 */
int SocketIO::send (char* buf,int len,int& s,ACE_Time_Value*) 
{
    char* tmpbuf = buf;
    int   tmplen = len;
    int n = write_out(s,PM_NULL);
    int s_len = length();
    if ( s_len == 0 ) 
    {
        //�������ڵ������Ѿ�������ɡ�
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
    
    //���������ʣ��ռ�
    s = space();

    return tmplen>0?(len-tmplen):len;
}

/**
 * ȡ�ã��������������д��ʼ��ַ��
 */
char* SocketIO::get_wr_ptr(){return m_output_buffer->wr_ptr();}
void  SocketIO::set_wr_ptr(int nBytes) {m_output_buffer->wr_ptr(nBytes);}

/**
 * ȡ�ã������������������ʼ��ַ��
 */
char* SocketIO::get_rd_ptr(){return m_output_buffer->rd_ptr();}
void  SocketIO::set_rd_ptr(int nBytes){m_output_buffer->rd_ptr(nBytes);}




/**
 * ȡ�����������ʣ��ռ��С��
 */
int SocketIO::space() {return m_output_buffer->space();}

/**
 * ȡ��������������ݳ��ȡ�
 */
int SocketIO::length(){return m_output_buffer->length();}

//�������������
void SocketIO::reset() {m_output_buffer->reset();}





