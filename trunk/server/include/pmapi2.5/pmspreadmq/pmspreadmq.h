/************************************************************************/
/* file:pmspreadmq.h
 * date:2008-08-17
 * auth:yangrui@leadtone.com
 * history:
************************************************************************/

#ifndef __PM_SPREAD_MQ__H__2008_08_17
#define __PM_SPREAD_MQ__H__2008_08_17

#ifdef WIN32
	#ifndef PMSPREADMQ_EXPORTS
        #pragma comment(lib,"pmspreadmq.lib")
	#endif
#endif

//#include <STRING>
#include <memory>
#include <map>
using namespace std;

/**	\brief	The PmSpreadMQ class
    Spread SP_Functions wrapper.
    ���������ļ�����Spread daemon������ָ������Ϣ���С�
    �����ṩ��ȡ��socket�����֣���ָ����Ϣ���з�����Ϣ��������Ϣ�Ľӿڡ�
    sample: pseudocode

    string module_name("FETCH_MAIL");//maybe use pmstrig
    string queue_conf("$CEF_HOME/conf/queue.conf");//maybe use pmstring
    PmSpreadQueuePtr spreadQPtr = PmCreateSpreadQueue(module_name,queue_conf);
    ��
    if ( select(fdsize,&readset,null,&exceptset,&timeout) >0 ) 
    {
	    ��
	    if ( FD_ISSET(fd,&readset) || FD_ISSET(fd,&exceptset) ) 
        {
		    len = receive(��);
		    if ( len == SPREAD_QUEUE_SUCCESS ) 
            {
	            //������Ϣ�ɹ���
            }
            else if( len == SPREAD_QUEUE_INNER_INFO ) 
            {
	            //��Ϣ�����ڲ���Ϣ�����ԣ�������
            }
            else 
            {
	            //ʧ�ܡ�
            }
        }
    }

*/
class PmSpreadMQ 
{
    /**	\brief	The PmSpreadMQConf class ��Ϣ���������ļ��ࡣ

    */
    class PmSpreadMQConf 
    {
    public :
        
        //���캯��
        PmSpreadMQConf(const PmString &queue_conf,const PmString &module_name );
        //��������
        //~PmSpreadMQConf(){};
    
    public :
    
        //ȡ�ø�ģ����������Ϣ���������б�
        //map<PmString,PmString> & get_queue_list() { return m_queue_list;};
        //ȡ�����е���Ϣ���С�
        map<PmString,PmString> & get_queue_all_list() { return m_queue_all_list;};

        //ȡ��Spread Daemon������
        const PmString & get_spread_name() const { return m_spread_name;};
        //ȡ������
        const PmString & get_private_name() const { return m_private_name;};
        
        //��Ҫ�������Ϣ�������ƣ�ʹ��','�ָ����ڼ�����ʱ��Ҫ������Ϣ�������ͼ��ϱ�Ҫ��ǰ׺��
        const PmString & get_group_list() const {return m_group_str;};

    private :
        //load config file
        int loadconf( const PmString &queue_conf );
    private :
        
        //������Ҫ�������Ϣ�������ơ�
        //map<PmString,PmString> m_queue_list;
        
        //����������Ϣ�������ơ�
        map<PmString,PmString> m_queue_all_list;
        
        //��Ҫ�������Ϣ�������ƣ�ʹ��','�ָ����ڼ�����ʱ��Ҫ������Ϣ�������ͼ��ϱ�Ҫ��ǰ׺��
        PmString m_group_str;
       
        //˽�����ơ�
        PmString m_private_name;

        //spread daemon name
        PmString m_spread_name;
    };

public:

    /**	\brief	The PmSpreadMQ function
                ���캯����

	    \param	module_name	a parameter of type const string &
                            ģ�����ơ�
	    \param	queue_conf	a parameter of type const string &
                            ��Ϣ���������ļ�·����

    */
    PmSpreadMQ(const PmString &module_name,const PmString &queue_conf);

    //����������
    ~PmSpreadMQ();

public:

    /**	\brief	The getsocket function ȡ�������ػ����̵�socket�����֡�


	    \return	const PM_SOCKET 

    */
    const PM_SOCKET getsocket() const { return m_mailbox;};

    /**	\brief	The sendmsg function ��ָ����Ϣ���з�����Ϣ��

	    \param	queue_name	a parameter of type const PmString & ��Ϣ�������ơ�

	    \param	mess_type	a parameter of type short  �ȴ����͵���Ϣ�����࣬����Ӧ�ò㶨�塣
	    \param	message	    a parameter of type char*  �ȴ����͵���Ϣ��
	    \param	mess_len	a parameter of type int  �ȴ����͵���Ϣ�ĳ��ȡ�

	    \return	int NORMAL   : ���͵����ݳ��ȡ�
                    -11      : ILLEGAL_SESSION��
                    -8       : CONNECTION_CLOSED��
                    -13      : ILLEGAL_MESSAGE��
	    
    */
    int sendmsg(const PmString &queue_name,short mess_type, const char * buffer ,int buffer_size);

    /**	\brief	The recvmsg function ������Ϣ��

	    \param	queue_name	a parameter of type PmString & ��Ϣ���Ե���Ϣ�������ơ�
	    \param	mess_type	a parameter of type short & ��Ϣ���͡�
	    \param	buffer	    a parameter of type char * ��������
	    \param	buffer_size	a parameter of type int & ��������С��

	    \return	int 0 : �ɹ�
                    1 : ���ԣ���Ϣ�����ڲ���Ϣ��
                    2 : ������С��
                    3 : ����

	    
    */
    int recvmsg(PmString &queue_name,short &mess_type,char * buffer ,int &buffer_size);

private :

    /**	\brief	The PmSpreadMQ::connectandjoin function
        connect to spread daemon and join queue

	    \return	int  1    :  ACCEPT_SESSION
                    -1    :  ILLEGAL_SPREAD
                    -2    :  COULD_NOT_CONNECT
                    -4    :  REJECT_NO_NAME
                    -5    :  REJECT_ILLEGAL_NAME
                    -6    :  REJECT_NOT_UNIQUE
                    -7    :  REJECT_VERSION
                    -8    :  CONNECTION_CLOSED
    */
    int connectandjoin() ;

    //disconnect
    void leaveanddisconnect();

private :
    
    //����Ҫ��ʾ�رա�
    PM_SOCKET m_mailbox;
    //��Ϣ��������
    PmSpreadMQConf m_conf;
};

//����PmSpreadQueue��auto point ����ʹ�ù���ע��ָ�������Ȩ�ڸ�ֵ�����з���ת�ơ�
typedef auto_ptr< PmSpreadMQ > PmSpreadQueuePtr;

/**	\brief	The PmCreateSpreadQueue function

	\param	module_name	a parameter of type const string & 
                        ʹ�ô���Ϣ���е�ģ�����ơ�
	\param	queue_conf	a parameter of type const string & 
                        ��Ϣ���������ļ�·����

	\return	PmSpreadQueuePtr ��������ָ�룬ָ�����ɵ���Ϣ����ʵ����

	
*/
PmSpreadQueuePtr PmCreateSpreadQueue(const PmString &module_name,const PmString &queue_conf);

/**
 * \brif pm spread mq info typedef
 */
typedef struct _st_spreadmq_info
{
    PM_SOCKET mailbox;//connection to spread daemon
    char spread_name[20];  //spread name :4803@xxx.xxx.xxx.xxx
    char private_name[10]; //must be unique on the machine running the spread daemon
                           //at most MAX_PRIVATE_NAME characters
		           //mainly it cannot contain the ��#�� character 
    char private_group[32];//private group name
}PM_SPREADMQ_INFO;

/** \brif Connect Spread Daemon. 
 *  \param PM_SPREADMQ_INFO ������Ϣ������Ϣ����������Spread Daemon�����Ӿ��������������ƣ�˽�����ƣ�˽�������� 
 *  \return int 1     :  ACCEPT_SESSION
 *              -1    :  ILLEGAL_SPREAD
 *              -2    :  COULD_NOT_CONNECT
 *              -4    :  REJECT_NO_NAME
 *              -5    :  REJECT_ILLEGAL_NAME
 *              -6    :  REJECT_NOT_UNIQUE
 *              -7    :  REJECT_VERSION
 *              -8    :  CONNECTION_CLOSED
 */
int PmSpreadMQ_Connect(PM_SPREADMQ_INFO&);

/** \brif Connect Spread Daemon with timeout. 
 *  \param PM_SPREADMQ_INFO ������Ϣ������Ϣ����������Spread Daemon�����Ӿ��������������ƣ�˽�����ƣ�˽�������� 
 *  \param long           second
 *  \param long           usec
 *  \return int 1     :  ACCEPT_SESSION
 *              -1    :  ILLEGAL_SPREAD
 *              -2    :  COULD_NOT_CONNECT
 *              -4    :  REJECT_NO_NAME
 *              -5    :  REJECT_ILLEGAL_NAME
 *              -6    :  REJECT_NOT_UNIQUE
 *              -7    :  REJECT_VERSION
 *              -8    :  CONNECTION_CLOSED
 */
int PmSpreadMQ_Connect_TimeOut(PM_SPREADMQ_INFO&,long,long);

/** \brif  Join Group
 *  \param PM_SPREADMQ_INFO  
 *  \param const char* group name,at most MAX PRIVATE NAME characters. 
 *  \return int 0      : success
 *              -14    : ILLEGAL_GROUP
 *              -13    : ILLEGAL_SESSION
 *              -8     : CONNECTION_CLOSED
 */
int PmSpreadMQ_Join(PM_SPREADMQ_INFO&,const char*);

/** \brif recv message 
 *  \param PM_SPREADMQ_INFO 
 *  \param char[32]          This will be set to the name of the sending connection(its private group name).
 *  \param short&            message type that the application sent with the original message
 *  \param char*             a buffer will recv message
 *  \param int&              [in] buffer length,[out] if buffer is too short it will be actural buffer length should alloc
 *  \return int NORMAL :     Returns the size of the message received on success.
 *              -8     :     CONNECTION_CLOSED
 *              -11    :     ILLEGAL_SESSION
 *              -13    :     ILLEGAL_MESSAGE
 *              -14    :     BUFFER_TOO_SHORT
 *              -100   :     Group inner message ,ignore.
 */
int PmSpreadMQ_Recv_Msg(PM_SPREADMQ_INFO&,char[32],char[32],short&,char*,int&);

/** \brif  send message
 *  \param  PM_SPREADMQ_INFO
 *  \param  const char*      group name
 *  \param  short            message type that the application sent with the original message
 *  \param  const char*      message
 *  \param  int              message length
 *  \return int  NORMAL   :  message length that has send.
 *               -8       :  CONNECTION_CLOSED
 *               -11      :  ILLEGAL_SESSION
 *               -13      :  ILLEGAL_MESSAGE
 */
int PmSpreadMQ_Send_Msg(PM_SPREADMQ_INFO&,const char*,short,char*,int&);

/** \brif leave group 
 *  \param PM_SPREADMQ_INFO
 *  \return int 0    :    success
 *              -14  :    ILLEGAL_GROUP
 *              -13  :    ILLEGAL_SESSION
 *              -8   :    CONNECTION_CLOSED
 */
int PmSpreadMQ_Leave(PM_SPREADMQ_INFO&,const char*);

/** \brif disconnection
 *  \param PM_SPREADMQ_INFO
 *  \return int 0   : success
 *              -11 : ILLEGAL_SESSION
 */
int PmSpreadMQ_Disconnect(PM_SPREADMQ_INFO&);





#define IID_IPmMessageSender		0x10001000
struct IPmMessageSender : IPmUnknown
{
	virtual void put(const char* pszGorupSendTo, const PmString& strData, short nMsgType ) = 0;
	virtual void put(const char* pszGorupSendTo, const char* pszData, short nMsgType ) = 0 ;
	virtual void put(const char* pszGorupSendTo, const void* pszData, int nDataLen, short nMsgType ) = 0;
};
typedef PmComSmartPtr<IPmMessageSender,IID_IPmMessageSender> PmMessageSenderPtr;

#define IID_IPmMessageTransferor	0x10001001
struct IPmMessageTransferor : IPmMessageSender
{
	virtual void get(PmString& , short& nMsgType ) = 0;
	virtual void getWithType(PmString&, short nMsgType ) = 0;
};
typedef PmComSmartPtr<IPmMessageTransferor,IID_IPmMessageTransferor> PmMessageTransferorPtr;


//void PmSpreadInitialize( const char* pszConfigFileName );

PmMessageSenderPtr PmCreateMessageSender( const char *pSpreadName="4803@127.0.0.1", int nTimeOut = PM_INFINITE );
PmMessageTransferorPtr PmCreateMessageTransferor( const char *pSpreadName, const char* pszGroupName, int nTimeOut = PM_INFINITE );

#endif

