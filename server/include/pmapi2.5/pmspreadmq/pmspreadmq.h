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
    根据配置文件连接Spread daemon，加入指定的消息队列。
    对外提供了取得socket描述字，向指定消息队列发送消息，接收消息的接口。
    sample: pseudocode

    string module_name("FETCH_MAIL");//maybe use pmstrig
    string queue_conf("$CEF_HOME/conf/queue.conf");//maybe use pmstring
    PmSpreadQueuePtr spreadQPtr = PmCreateSpreadQueue(module_name,queue_conf);
    …
    if ( select(fdsize,&readset,null,&exceptset,&timeout) >0 ) 
    {
	    …
	    if ( FD_ISSET(fd,&readset) || FD_ISSET(fd,&exceptset) ) 
        {
		    len = receive(…);
		    if ( len == SPREAD_QUEUE_SUCCESS ) 
            {
	            //接收消息成功。
            }
            else if( len == SPREAD_QUEUE_INNER_INFO ) 
            {
	            //消息队列内部消息，忽略，不处理。
            }
            else 
            {
	            //失败。
            }
        }
    }

*/
class PmSpreadMQ 
{
    /**	\brief	The PmSpreadMQConf class 消息队列配置文件类。

    */
    class PmSpreadMQConf 
    {
    public :
        
        //构造函数
        PmSpreadMQConf(const PmString &queue_conf,const PmString &module_name );
        //析构函数
        //~PmSpreadMQConf(){};
    
    public :
    
        //取得该模块所属的消息队列名称列表
        //map<PmString,PmString> & get_queue_list() { return m_queue_list;};
        //取得所有的消息队列。
        map<PmString,PmString> & get_queue_all_list() { return m_queue_all_list;};

        //取得Spread Daemon的名称
        const PmString & get_spread_name() const { return m_spread_name;};
        //取得名。
        const PmString & get_private_name() const { return m_private_name;};
        
        //需要加入的消息队列名称，使用','分隔，在加入域时需要根据消息队列类型加上必要的前缀。
        const PmString & get_group_list() const {return m_group_str;};

    private :
        //load config file
        int loadconf( const PmString &queue_conf );
    private :
        
        //保存需要加入的消息队列名称。
        //map<PmString,PmString> m_queue_list;
        
        //保存所有消息队列名称。
        map<PmString,PmString> m_queue_all_list;
        
        //需要加入的消息队列名称，使用','分隔，在加入域时需要根据消息队列类型加上必要的前缀。
        PmString m_group_str;
       
        //私有名称。
        PmString m_private_name;

        //spread daemon name
        PmString m_spread_name;
    };

public:

    /**	\brief	The PmSpreadMQ function
                构造函数。

	    \param	module_name	a parameter of type const string &
                            模块名称。
	    \param	queue_conf	a parameter of type const string &
                            消息队列配置文件路径。

    */
    PmSpreadMQ(const PmString &module_name,const PmString &queue_conf);

    //析构函数。
    ~PmSpreadMQ();

public:

    /**	\brief	The getsocket function 取得连接守护进程的socket描述字。


	    \return	const PM_SOCKET 

    */
    const PM_SOCKET getsocket() const { return m_mailbox;};

    /**	\brief	The sendmsg function 向指定消息队列发送消息。

	    \param	queue_name	a parameter of type const PmString & 消息队列名称。

	    \param	mess_type	a parameter of type short  等待发送的消息的种类，可由应用层定义。
	    \param	message	    a parameter of type char*  等待发送的消息。
	    \param	mess_len	a parameter of type int  等待发送的消息的长度。

	    \return	int NORMAL   : 发送的数据长度。
                    -11      : ILLEGAL_SESSION。
                    -8       : CONNECTION_CLOSED。
                    -13      : ILLEGAL_MESSAGE。
	    
    */
    int sendmsg(const PmString &queue_name,short mess_type, const char * buffer ,int buffer_size);

    /**	\brief	The recvmsg function 接收消息。

	    \param	queue_name	a parameter of type PmString & 消息来自的消息队列名称。
	    \param	mess_type	a parameter of type short & 消息类型。
	    \param	buffer	    a parameter of type char * 缓冲区。
	    \param	buffer_size	a parameter of type int & 缓冲区大小。

	    \return	int 0 : 成功
                    1 : 忽略，消息队列内部消息。
                    2 : 缓冲区小。
                    3 : 错误。

	    
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
    
    //不需要显示关闭。
    PM_SOCKET m_mailbox;
    //消息队列配置
    PmSpreadMQConf m_conf;
};

//定义PmSpreadQueue的auto point ，在使用过程注意指针的所有权在赋值操作中发生转移。
typedef auto_ptr< PmSpreadMQ > PmSpreadQueuePtr;

/**	\brief	The PmCreateSpreadQueue function

	\param	module_name	a parameter of type const string & 
                        使用此消息队列的模块名称。
	\param	queue_conf	a parameter of type const string & 
                        消息队列配置文件路径。

	\return	PmSpreadQueuePtr 返回智能指针，指向生成的消息队列实例。

	
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
		           //mainly it cannot contain the ’#’ character 
    char private_group[32];//private group name
}PM_SPREADMQ_INFO;

/** \brif Connect Spread Daemon. 
 *  \param PM_SPREADMQ_INFO 保存消息队列信息，包括：与Spread Daemon的连接句柄，加入的组名称，私有名称，私有组名称 
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
 *  \param PM_SPREADMQ_INFO 保存消息队列信息，包括：与Spread Daemon的连接句柄，加入的组名称，私有名称，私有组名称 
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

