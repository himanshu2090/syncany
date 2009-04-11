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
     * ���캯����
     * 
     * ACE_SOCK_Stream       sock stream 
     * ACE_Message_Block     ���뻺����
     * ACE_Message_Block     ���������
     */
    SocketIO( ACE_SOCK_Stream * stream,ACE_Message_Block *input,ACE_Message_Block *output)
                   :m_pAceStream(stream),
                    m_input_buffer(input),
                    m_output_buffer(output),
                    m_flag_recv_fin(0),
					m_iremoteport(0)
	{
	};
    
    //���캯����
    ~SocketIO(){};

public :

    /**
     * ��socket stream�ж�ȡ���ݡ�
     * return : 0   recevie FIN 
     *          >0  recevie data length
     *          -1  ʧ��
     */
    int recv();

    /**
     * �ӻ������ж�ȡ�����ȡ������һ�����
     * 
     * string          �����ȡ�����
     * ACE_Time_Value  ���á�
     * return : 0   recevie FIN 
     *          >0  recevie data length
     *          -1  û�ж���������һ�����
     */
    int get_line(string& line,ACE_Time_Value* = NULL);

    int get_line(PmString& line,ACE_Time_Value* time = NULL);

    /**
     * ���ͻ������ڵ����ݡ�
     *  
     * int&           ���ػ�����ʣ�೤�ȡ�
     * ACE_Time_Value 
     *
     * return         �Ѿ����͵����ݡ�
     */
    int write_out(int&,ACE_Time_Value*);
	int write_all_out(ACE_Time_Value* pTime = NULL);
	int write_to_cache(const PmString& strOutPut);

	/*
	 * strBuf ��������Ͳ������¶�ȡ�����ݽ���ӵ����
	 * return ���ζ�ȡ�ĳ���
	 */
	int read_all_append(PmString& strBuf,ACE_Time_Value* pTime = NULL);
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
    int send (char*,int,int&,ACE_Time_Value*);

    /**
     * ȡ�ã��������������д��ʼ��ַ��
     */
    char* get_wr_ptr();
    void  set_wr_ptr(int nBytes);
    
    /**
     * ȡ�ã������������������ʼ��ַ��
     */
    char* get_rd_ptr();
    void  set_rd_ptr(int nBytes);

    /**
     * ȡ�����������ʣ��ռ��С��
     */
    int space();

    /**
     * ȡ��������������ݳ��ȡ�
     */
    int length();
 
    //�������������
    void reset();


	//�жϻ������Ƿ����������һ��
	PM_BOOL hasLine();

	int m_iremoteport;	

private :

    //ace socket stream
    ACE_SOCK_Stream * m_pAceStream;

    //���뻺����ȱʡ1k�ֽڣ����������ȱʡ16k�ֽڡ�
    //Ŀǰ��ʹ��һ���������飬�Ժ������չ��ʹ�ö���������飬������Ҫ��̬�����ӡ�
    ACE_Message_Block *m_input_buffer;
    ACE_Message_Block *m_output_buffer;
    
    //
    int m_flag_recv_fin ;
};

#endif


