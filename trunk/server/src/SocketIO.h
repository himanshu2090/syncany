#ifndef __SOCKET_IO_H__
#define __SOCKET_IO_H__

#define PMPROTO_MAX_CMD_BUF 1024

class ACE_Time_Value;
class ACE_SOCK_Stream;
class ACE_Message_Block;

class SocketIO 
{
public :
    
    /**
     * 构造函数。
     * 
     * ACE_SOCK_Stream       sock stream 
     * ACE_Message_Block     输入缓冲区
     * ACE_Message_Block     输出缓冲区
     */
    SocketIO( ACE_SOCK_Stream * stream,ACE_Message_Block *input,ACE_Message_Block *output)
                   :m_pAceStream(stream),
                    m_input_buffer(input),
                    m_output_buffer(output),
                    m_flag_recv_fin(0),
					m_iremoteport(0)
	{
	};
    
    //构造函数。
    ~SocketIO(){};

public :

    /**
     * 从socket stream中读取数据。
     * return : 0   recevie FIN 
     *          >0  recevie data length
     *          -1  失败
     */
    int recv();

    /**
     * 从缓冲区中读取命令，读取完整的一行命令。
     * 
     * string          保存读取的命令。
     * ACE_Time_Value  不用。
     * return : 0   recevie FIN 
     *          >0  recevie data length
     *          -1  没有读到完整的一行命令。
     */
    int get_line(string& line,ACE_Time_Value* = NULL);

    int get_line(PmString& line,ACE_Time_Value* time = NULL);

    /**
     * 发送缓冲区内的数据。
     *  
     * int&           返回缓冲区剩余长度。
     * ACE_Time_Value 
     *
     * return         已经发送的数据。
     */
    int write_out(int&,ACE_Time_Value*);
	int write_all_out(ACE_Time_Value* pTime = NULL);
	int write_to_cache(const PmString& strOutPut);

	/*
	 * strBuf 输入输出型参数，新读取的内容将添加到最后
	 * return 本次读取的长度
	 */
	int read_all_append(PmString& strBuf,ACE_Time_Value* pTime = NULL);
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
    int send (char*,int,int&,ACE_Time_Value*);

    /**
     * 取得，设置输出缓冲区写起始地址。
     */
    char* get_wr_ptr();
    void  set_wr_ptr(int nBytes);
    
    /**
     * 取得，设置输出缓冲区读起始地址。
     */
    char* get_rd_ptr();
    void  set_rd_ptr(int nBytes);

    /**
     * 取得输出缓冲区剩余空间大小。
     */
    int space();

    /**
     * 取得输出缓冲区数据长度。
     */
    int length();
 
    //重置输出缓冲区
    void reset();


	//判断缓存中是否存在完整的一行
	PM_BOOL hasLine();

	int m_iremoteport;	

private :

    //ace socket stream
    ACE_SOCK_Stream * m_pAceStream;

    //输入缓冲区缺省1k字节，输出缓冲区缺省16k字节。
    //目前仅使用一个缓冲区块，以后可以扩展，使用多个缓冲区块，根据需要动态的增加。
    ACE_Message_Block *m_input_buffer;
    ACE_Message_Block *m_output_buffer;
    
    //
    int m_flag_recv_fin ;
};

#endif


