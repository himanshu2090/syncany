#ifndef __PM_SCL_SOCKET_H__
#define __PM_SCL_SOCKET_H__

#define PMSOCK_ERROR_TIMEOUT	-1
#define PMSOCK_ERROR_OTHER		-2
#define PMSOCK_ERROR_EOF		-3
//*    -1 on timeout
//*    -2 on other error
//*    -3 on reaching the END

//定义二级缓存大小
#define	PM_SOCK_RECVBUF_SIZE	8192


/**	\brief	The PmSocketBase class

*/

struct	PM_SOCKET_INFO;

class PmSocketBase
{
protected:
	PM_SOCKET_INFO * m_sock_info;
	PM_DWORD m_dwRecvTimeout,m_dwSendTimeout;

public:
	PmSocketBase();
	virtual ~PmSocketBase();
    virtual PM_SOCKET getFd()=0;
    virtual PM_BOOL	isValid()=0;
    virtual void attach(PM_SOCKET sockfd)=0;
    virtual PM_SOCKET detach()=0;
	virtual void close();
//	virtual PM_BOOL create(PM_DWORD dwRecvTimeout=30,PM_DWORD dwSendTimeout=30)=0;
//	virtual PM_BOOL getSockInfo(PM_DWORD& ip ,PM_UINT & port)=0;

/**	\brief	The getSockInfo function 获得socket的信息

	\param	ip	a parameter of type PM_IPADDR_V4 &
	\param	port	a parameter of type PM_UINT &

	\return	PM_BOOL
*/
	PM_BOOL getSockInfo(PM_IPADDR_V4 & ip ,PM_UINT & port);
	PM_BOOL getPeerSockInfo(PM_IPADDR_V4 & ip ,PM_UINT & port);
	virtual void setTimeOut( PM_DWORD nRecv, PM_DWORD nSend );
	void getTimeOut( PM_DWORD* r, PM_DWORD* s);
protected:
	PM_VOID clearBuffer();
};

/**	\brief	The PmTcpSocket class

*/
class PmTcpSocket : public PmSocketBase
{
public:
	PmTcpSocket();
	PmTcpSocket( PM_BOOL bSslEnable );

    virtual PM_SOCKET getFd();
    virtual PM_BOOL	isValid();
    virtual void attach(PM_SOCKET sockfd);
    virtual PM_SOCKET detach();

    /* check if connected */
    virtual PM_BOOL	connected();

    virtual PM_BOOL	open(const char* host, PM_PORT port);
	virtual PM_BOOL listen( PM_PORT port );
	virtual PM_BOOL accept(PmTcpSocket&);
    
    virtual int read(ByteStr* buf);
    virtual int read(char *buf, int size);
    virtual int timed_read(char* buf, int maxcnt, int time_out);
    virtual int timed_read_all(char* buf, int maxcnt, int time_out);
    virtual int	readline(char *, int, int);
    virtual int readline(ByteStr *buf, int timeout);

    virtual int	write(ByteStr*);
    virtual int write(const void *data, int size);
    virtual int	puts(const char *);
	virtual int prints(const char *fmt, ...);

//读取时使用设置的超时设置
	virtual int readEx(char *buf, int size);
	virtual int	readlineEx(char *, int );
	virtual int readlineEx(ByteStr *buf );
	void closeLineoff();
	int readn( char* buf, int size );
	virtual void close();
	void enableSsl( PM_BOOL bEnable );
protected:
	PM_INT readAPack( int time_out );
	int readReal(char* buf, int maxcnt, int time_out);
	int readFromBuf(char* buf, int maxcnt );
private:
	PM_BOOL sslConnect();
	PM_BOOL sslClose();
	int sslWrite(char* buf, int size);
	int sslRead(char* buf, int size);
};



#endif
