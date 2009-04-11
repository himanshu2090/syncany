#ifndef __PM_SCL_INTERFACE_DEF_H__
#define __PM_SCL_INTERFACE_DEF_H__

/**	
 *@file PmInterfaceDef.h 
 *@brief pmapi接口定义
*/
/** @defgroup pmapi_interface pmapi所有接口定义
 *  这是pmapi的接口定义
 *  @{
 */

/**	\brief mysql数据库接口导出的类名
*/
#define CLSID_PM_DBCONN_MYSQL "CLSID_PM_DBCONN_MYSQL"

/**	\brief 数据库连接的接口ID
*/
#define IID_IPmDbConnection		0x10000002
/**	\brief 数据库访问结果集合的接口ID
*/
#define IID_IPmResultSet		0x10000003
/**	\brief 消息队列接口ID
*/
#define IID_IPmMessageQueue		0x10000004
/**	\brief 线程的接口ID
*/
#define IID_IPmThread			0x10000005
/**	\brief 服务的接口ID
*/
#define IID_IPmService			0x10000006
/**	\brief 异常的接口ID
*/
#define IID_IPmException		0x10000007
/**	\brief 同步对象的接口ID
*/
#define IID_IPmSynchronizer		0x10000008
/**	\brief 输入流的接口ID
*/
#define IID_IPmInputStream		0x10000009
/**	\brief 输入流允许的接口ID
*/
#define IID_IPmInputStreamable	0x1000000a
/**	\brief 条件量对象的接口ID
*/
#define IID_IPmCondition		0x1000000b
/**	\brief 属性列表的接口ID
*/
#define IID_IPmProperties		0x1000000c
/**	\brief 检查器的接口ID
*/
#define IID_IPmInspector		0x1000000d
/**	\brief 输出流的接口ID
*/
#define IID_IPmOutputStream		0x1000000e
/**	\brief 输出流允许的接口ID
*/
#define IID_IPmOutputStreamable 0x1000000f

#define IID_IPmPrintStream		0x10000010
#define IID_IPmPrintStreamable	0x10000011
#define IID_IPmClassFactory		0x10000012

#define IID_IPmMessageQueueEx	0x10000013
#define IID_IPmThreadPool		0x10000014
#define IID_IPmProcess			0x10000015

#define IID_IPmStringList		0x10000016
#define IID_IPmMapStringToString	0x10000017

#define IID_IPmMessageQueueListener 	0x10000018
#define IID_IPmDbConnectionEx		0x10000019

#define IID_IPmProcessEx		0x10000020

/**	\brief	数据库操作结果集接口
	\note	数据库操作访问数据时通过此接口,此接口的实例都是通过IPmDbConnection::query来生成的,
			本对象代表了数据库的一条记录,可以通过getAt或者operator []来获得某列的值。
			如果操作过程中发生了错误，抛出PmException异常
	\code 访问数据库代码示例

	PmString strSql;
	strSql.format(
      "SELECT trx_id, email_id, trx_state, email_size, pushuser_id "
      "FROM trx_history t "
      "WHERE trx_type=%d and trx_state<>255 and trx_state<>%d and trx_state<>%d ",
	  0, 6,8);

	try
    {
		PmResultSetPtr rs;
		rs = ptrDbConnection->query(strSql);
		while (rs->fetchRow())
        {
			pm_printf("%d %d %d %s\r\n", pm_atol(rs->getAt(0)),
				 pm_atol(rs->getAt(1),
				 pm_atol(rs->getAt(2),
				 rs->getAt(3) );
        }
    }
	catch (PmException& ex)
    {
		pm_error("db query fail:%s", ex->getText().c_str());
		//or ex->append( ... ); throw ex;
    }
	\endcode
	\see IPmDbConnection
*/
struct IPmResultSet : IPmUnknown
{
/**	\brief	The getRowCount function 获得所有记录的行数
	\return	PM_ULONG 记录行数
*/
	virtual PM_ULONG getRowCount()=0;
/**	\brief	The fetchRow function 获得下一行记录
	\return	PM_BOOL 返回是否已经还有记录，如果是PM_FALSE表示已经到尾没有更多记录，否则还有，如果数据访问错误，抛出PmException异常
*/
	virtual PM_BOOL fetchRow()=0;
/**	\brief	The getAt function 获得当前行某列的数据，首先必须成功调用fetchRow
	\param[in]	iIndex	a parameter of type int 列序号，0开始，调用者必须自己保证不越界
	\return	const char* 返回常量字符串指针
*/
	virtual const char *getAt(int iIndex)=0;
/**	\brief	The [] function 获得当前行某列的数据，首先必须成功调用fetchRow
	\param[in]	iIndex	a parameter of type int 列序号，0开始，调用者必须自己保证不越界
	\return	const char *operator
*/
	virtual const char *operator [](int iIndex)=0;
/**	\brief	The getColumnCount function 获得列的数目
	\return	PM_ULONG 列数
*/
	virtual PM_ULONG getColumnCount() = 0;
};
/**	\brief	Redefinition of PmComSmartPtr<IPmResultSet,IID_IPmResultSet> as PmResultSetPtr
*/
typedef PmComSmartPtr<IPmResultSet,IID_IPmResultSet> PmResultSetPtr;


/**	\brief	数据库连接接口

*/
struct IPmDbConnection : IPmUnknown
{
/**	\brief	The connect function
	\param	pUri a parameter of type const PmUri*, 通过此uri获得连接的信息 uri类似mysql://root:pwd\@localhost/cef_push
	\return	virtual void	
*/
	virtual void connect(const PmUri* pUri)=0;
/**	\brief	The disconnect function

	\return	virtual void
	
*/
	virtual void disconnect()=0;
/**	\brief	The query function

	\param	sqlstmt	a parameter of type PM_LPCSTR

	\return	virtual PmResultSetPtr

	
*/
	virtual PmResultSetPtr query(PM_LPCSTR sqlstmt)=0;
/**	\brief	The execute function

	\param	sqlstmt	a parameter of type PM_LPCSTR

	\return	virtual void

	
*/
	virtual void execute(PM_LPCSTR sqlstmt)=0;

//	virtual PM_BOOL EscapeString(ByteStr *data)=0;
};

struct IPmDbConnectionEx : IPmUnknown
{
/**	\brief	The connect function
	\param	pUri a parameter of type const PmUri*, 通过此uri获得连接的信息 uri类似mysql://root:pwd\@localhost/cef_push
	\return	virtual void	
*/
	virtual void connectEx(const PmUri* pUri, const char* pszFile, int nFileLine )=0;
/**	\brief	The disconnect function

	\return	virtual void
	
*/
	virtual void disconnectEx(const char* pszFile, int nFileLine )=0;
/**	\brief	The query function

	\param	sqlstmt	a parameter of type PM_LPCSTR

	\return	virtual PmResultSetPtr

	
*/
	virtual PmResultSetPtr queryEx(PM_LPCSTR sqlstmt, const char* pszFile, int nFileLine )=0;
/**	\brief	The execute function

	\param	sqlstmt	a parameter of type PM_LPCSTR

	\return	virtual void

	
*/
	virtual void executeEx(PM_LPCSTR sqlstmt, const char* pszFile, int nFileLine )=0;

//	virtual PM_BOOL EscapeString(ByteStr *data)=0;
};

/**	\brief	Redefinition of PmComSmartPtr<IPmDbConnection,IID_IPmDbConnection> as PmDbConnectionPtr
*/
typedef PmComSmartPtr<IPmDbConnection,IID_IPmDbConnection> PmDbConnectionPtr;
typedef PmComSmartPtr<IPmDbConnectionEx,IID_IPmDbConnectionEx> PmDbConnectionExPtr;



//struct IPmDbManager : IPmUnknown
//{
//	virtual PmDbConnectionPtr getConnection(const PmUri*)=0;
//};
// typedef PmComSmartPtr<IPmDbManager,IID_IPmDbManager> PmDbManagerPtr;


class PmMqMessage;

/**	\brief	消息队列接口

*/
struct IPmMessageQueue : IPmUnknown
{
	virtual PM_INT get(PmMqMessage& pMsg) = 0;
	virtual PM_INT put(PmMqMessage& pMsg) = 0;
};
/**	\brief	Redefinition of PmComSmartPtr<IPmMessageQueue,IID_IPmMessageQueue> as PmMessageQueuePtr
*/
typedef PmComSmartPtr<IPmMessageQueue,IID_IPmMessageQueue> PmMessageQueuePtr;


/**	\brief 线程接口

*/
struct IPmThread : IPmUnknown
{
//The previous suspend count of the thread if successful; 0xFFFFFFFF otherwise,. 
//If the return value is zero, the current thread was not suspended. 
//If the return value is one, the thread was suspended, but is now restarted. 
//Any return value greater than one means the thread remains suspended
	virtual PM_DWORD resumeThread() = 0;
	virtual PM_DWORD suspendThread() = 0;
	virtual PM_HANDLE getThreadHandle()=0;
	virtual PM_UINT getThreadId()=0;
};
/**	\brief	Redefinition of PmComSmartPtr<IPmThread,IID_IPmThread> as PmThreadPtr
*/
typedef PmComSmartPtr<IPmThread,IID_IPmThread> PmThreadPtr;


/**	\brief	同步接口

*/
struct IPmSynchronizer : IPmUnknown
{
	virtual PM_VOID wait()=0;
	virtual PM_BOOL waitTimeout( PM_INT nSeconds )=0;
	virtual PM_VOID signal()=0; 
	virtual PM_HANDLE getHandle()=0;
};
/**	\brief	Redefinition of PmComSmartPtr<IPmSynchronizer,IID_IPmSynchronizer> as PmSynchronizerPtr
*/
typedef PmComSmartPtr<IPmSynchronizer,IID_IPmSynchronizer> PmSynchronizerPtr;

/**	\brief	同步条件接口

*/
struct IPmCondition : IPmUnknown
{
	virtual PM_VOID wait(PmSynchronizerPtr ptrMutex)=0;
	virtual PM_BOOL waitTimeout(PmSynchronizerPtr ptrMutex, PM_INT nSeconds )=0;
	virtual PM_VOID signal()=0;
	virtual PM_VOID broadcast()=0;
	virtual PM_HANDLE getHandle()=0;
};
/**	\brief	Redefinition of PmComSmartPtr<IPmCondition,IID_IPmCondition> as PmConditionPtr
*/
typedef PmComSmartPtr<IPmCondition,IID_IPmCondition> PmConditionPtr;


/**	\brief	服务接口

*/
struct IPmService : IPmUnknown
{
/**	\brief	The stop function

	\return	virtual void	
*/
	virtual void stop()=0;

/**	\brief	The initialize function

	\return	virtual PM_BOOL	
*/
	virtual PM_BOOL initialize()=0;

/**	\brief	服务运行函数

	\param	hData	a parameter of type PM_HANDLE

	\return	virtual PM_INT	
*/
	virtual PM_INT run( PM_HANDLE hData, PM_HANDLE hUserData = 0 )=0;
};
/**	\brief	Redefinition of PmComSmartPtr<IPmService,IID_IPmService> as PmServicePtr
*/
typedef PmComSmartPtr<IPmService,IID_IPmService> PmServicePtr;


/**	\brief	The PM_EXCEPTION_ITEM struct

*/
struct PM_ERROR_DETAIL
{
	char szFunctionName[255];
	char szSourceFile[PM_MAX_PATH];
	PM_UINT nLine;
	char szError[4096];
	PM_DWORD dwErrno;
	PM_DWORD dwErrnoSystem;
};

/**	\brief	The 异常接口

*/
struct IPmException : IPmUnknown
{
	virtual void append( const char* pszFunctionName, const char* pszSrcFileName, int nSrcFileLine, 
		PM_DWORD, const char* ) = 0;
	virtual void appendFmt( const char* pszFunctionName, const char* pszSrcFileName, int nSrcFileLine, 
		PM_DWORD, const char*, ... ) = 0;
	virtual void vappend( const char* pszFunctionName, const char* pszSrcFileName, int nSrcFileLine, 
		PM_DWORD, const char*, PM_ARGLIST args ) = 0;
	virtual PmString getText()=0;
	virtual PmString getTopText()=0;
	virtual PM_DWORD getLastErrorNo()=0;
	virtual PM_ERROR_DETAIL* getLastError()=0;
	virtual void rewindError()=0;
	virtual PM_BOOL getNextError(PM_ERROR_DETAIL**)=0;
};
/**	\brief	Redefinition of PmComSmartPtr<IPmException, IID_IPmException> as PmExceptionPtr
*/
typedef PmComSmartPtr<IPmException, IID_IPmException> PmExceptionPtr;
/**	\brief	Redefinition of PmExceptionPtr as PmException
*/
typedef PmExceptionPtr PmException;

/**	\brief	流输入接口

*/
struct IPmInputStream : IPmUnknown
{
	/*!
	    \brief the number of bytes that can be read (or skipped over) from this 
		input stream without blocking by the next caller of a method for this input stream. 
	*/ 
	virtual PM_UINT available()=0;
		  
	/*!
	    \brief  Reads some number of bytes from the input stream and stores them into the buffer array b. 
	*/
	virtual PM_INT read(PM_UCHAR *b, PM_INT maxlen)=0;
};
/**	\brief	Redefinition of PmComSmartPtr<IPmInputStream, IID_IPmInputStream> as PmInputStreamPtr
*/
typedef PmComSmartPtr<IPmInputStream, IID_IPmInputStream> PmInputStreamPtr;



struct IPmOutputStream : IPmUnknown
{
	virtual PM_UINT write( PM_UCHAR* b, PM_INT maxlen )=0;
	virtual PM_VOID flush( )=0;
};
typedef PmComSmartPtr<IPmOutputStream, IID_IPmOutputStream> PmOutputStreamPtr;

struct IPmInputStreamable : IPmUnknown
{
	virtual PmVariant getInputStream()=0;
};
/**	\brief	Redefinition of PmComSmartPtr<IPmInputStreamable, IID_IPmInputStreamable> as PmInputStreamablePtr
*/
typedef PmComSmartPtr<IPmInputStreamable, IID_IPmInputStreamable> PmInputStreamablePtr;
struct IPmOutputStreamable : IPmUnknown
{
	virtual PmVariant getOutputStream()=0;
};
typedef PmComSmartPtr<IPmOutputStreamable, IID_IPmOutputStreamable> PmOutputStreamablePtr;

/**	\brief	The IPmProperties struct 属性列表，能根据属性名称设置和获取属性值
	\note	属性为名字对方式,名称=>值, 名称和值都不处理字符集信息。
			名称不要采用中文,前后不要有空格。
			名称大小写相关。
*/
struct IPmProperties : IPmUnknown
{
	/**	\brief	设置属性值
		\param[in]	pszProtertyName	属性名称(要具有唯一型)
		\param[in]	pmstrPropertyValue 属性值	
		\return	void
		*/
	virtual void setProperty( PM_LPCSTR pszProtertyName, PmString pmstrPropertyValue )=0;

	/**	\brief	根据属性名称提取属性值
		\param[in]	pszProtertyName	属性名称
		\return PmString 该名称所对应的属性值
				如果此名称的属性不存在，应该抛出PmException异常,错误码PM_ERROR_PROPERTY_NOTEXIST
	*/
	virtual PM_LPCSTR getProperty( PM_LPCSTR pszProtertyName )=0;

	/**	\brief 	重置遍历属性的游标指针
		\note	第一次调用getNextProperty函数前,要先调用该函数，否则有可能取不全所有属性
		\return	void		
	*/
	virtual void rewindProperties()=0;

	/**	\brief	获取下一个属性值
		\param[out]	strName		属性名称
		\param[out]	strValue	属性值
		\return	PM_BOOL:取到属性名字值对返回PM_TRUE;到了结尾返回PM_FALSE	
	*/
	virtual PM_BOOL getNextProperty( PmString& strName, PmString& strValue )=0;
};
/**	\brief	Redefinition of PmComSmartPtr<IPmProperties, IID_IPmProperties> as PmPropertiesPtr IPmProperties的智能指针
*/
typedef PmComSmartPtr<IPmProperties, IID_IPmProperties> PmPropertiesPtr;

struct IPmInspector : IPmUnknown
{
	virtual PM_BOOL isChanged()=0;
};
typedef PmComSmartPtr<IPmInspector,IID_IPmInspector> PmInspectorPtr;

struct IPmPrintStream : IPmUnknown
{
	virtual void vPrint( PM_LPCSTR pszFormat, PM_ARGLIST args )=0;
	virtual void print( PM_LPCSTR pszFormat, ... ) = 0;
	virtual void println( PM_LPCSTR pszFormat, ... ) = 0;
};
typedef PmComSmartPtr<IPmPrintStream,IID_IPmPrintStream> PmPrintStreamPtr;

struct IPmPrintStreamable : IPmUnknown
{
	virtual PmVariant getPrintStream()=0;	
};
typedef PmComSmartPtr<IPmPrintStreamable,IID_IPmPrintStreamable> PmPrintStreamablePtr;

struct IPmClassFactory : IPmUnknown
{
	virtual PmUnknownPtr createInstance( PM_IID iid, PM_DWORD dwContxt ) = 0;
};
typedef PmComSmartPtr<IPmClassFactory,IID_IPmClassFactory> PmClassFactoryPtr;


#define PM_MQ_MESSAGE_MAXLEN 8192
struct IPmMessageQueueEx : IPmUnknown
{
	virtual PM_INT peek( void* buf, int *pLen ) = 0;
	virtual void get( void* buf, int *pLen ) = 0;
	virtual PM_INT tryGet( void* buf, int *pLen ) = 0;
	virtual void put( void* buf, int len ) = 0;
	virtual PM_INT tryPut( void* buf, int len ) = 0;
	virtual void close() = 0;
};
typedef PmComSmartPtr<IPmMessageQueueEx,IID_IPmMessageQueueEx> PmMessageQueueExPtr;

typedef void (* PMMQ_ON_MESSAGE_FUNCTION)(const char* buf, int len );
typedef void (* PMMQ_ON_EXCEPTION_FUNCTION)( int eno, const char* pszErrInfo  );

struct IPmMessageQueueListener : IPmUnknown
{
	virtual void run( PM_HANDLE hStopEvent,PMMQ_ON_MESSAGE_FUNCTION onMessage, PMMQ_ON_EXCEPTION_FUNCTION onException  ) = 0;
};
typedef PmComSmartPtr<IPmMessageQueueListener,IID_IPmMessageQueueListener> PmMessageQueueListenerPtr;


/**	\brief	The (*PF_PM_TASK_HANDLER) function

	\param	pdata	a parameter of type void*
		从队列里取得的队列成员的数据
	\param	datalen	a parameter of type int
		从队列里取得的队列成员的数据长度
	\param	hStopEvent	a parameter of type PM_HANDLE
		退出事件,被设置以后本函数应该尽快退出
	\param	dwUserData	a parameter of type PM_DWORD
		用户数据,initThreadPool时传入的参数
	\return	typedef int
		返回0表示处理成功,返回-1表示成立中发生严重错误,应该退出,本函数不应该抛出异常
	\remark 本函数类型是任务回调函数

*/

typedef int (*PF_PM_TASK_HANDLER)( void* pdata, int datalen, PM_HANDLE hStopEvent, PM_DWORD dwUserData );

#define PM_THREAD_POOL_DISPATCH_SUCCESS	0
#define PM_THREAD_POOL_DISPATCH_BUSY	2
#define PM_THREAD_POOL_DISPATCH_ERROR	-1

struct IPmThreadPool : IPmUnknown
{
/**	\brief	The initThreadPool function

	\param	nMaxThreadCount	a parameter of type int
		线程池的线程数量
	\param	pfTaskHandler	a parameter of type PF_PM_TASK_HANDLER
		任务回调函数
	\param	hStopEvent	a parameter of type PM_HANDLE
		停止事件
	\param	dwUserData	a parameter of type PM_DWORD
		用户数据
	\return	virtual void
	\remark 请保证此函数在dispatchTask之前执行,并且在dispatchTask之后不被执行,如果违反,将抛出异常
		执行本函数时,会开出nMaxThreadCount个线程,等待任务分发
		当分发一个任务后,某一个线程会获得这个任务,回调函数是在线程中回调的
		在回调函数中,如果发现hStopEvent被置,应尽快退出
*/

	virtual void initThreadPool( int nMaxThreadCount, PF_PM_TASK_HANDLER pfTaskHandler, PM_HANDLE hStopEvent, PM_DWORD dwUserData ) = 0;
/**	\brief	The dispatchTask function
	\param	pdata	a parameter of type void*
		分发的任务关联的信息,长度应该小于一个队列消息能够接受的大小(PM_MQ_MESSAGE_MAXLEN-64)
	\param	datalen	a parameter of type int
		分发的任务关联的信息的长度
	\return	int
		可能有以下几种返回值
		PM_THREAD_POOL_DISPATCH_SUCCESS	分发成功
		PM_THREAD_POOL_DISPATCH_RUNNING 此任务正在跑
		PM_THREAD_POOL_DISPATCH_ERROR	有错应该终止(比如hStopEvent被置,或者发生其它严重错误)
	\remark
		当不能分发任务了，本函数将会等待，一直等到有空闲或者hStopEvent被置
		本函数不会抛出异常
*/
	virtual int dispatchTask( void* pdata, int datalen ) = 0;
/**	\brief	The tryDispatchTask function
	\param	pdata	a parameter of type void*
		分发的任务关联的信息,长度应该小于一个队列消息能够接受的大小(PM_MQ_MESSAGE_MAXLEN-64)
	\param	datalen	a parameter of type int
		分发的任务关联的信息的长度
	\return	virtual int
		PM_THREAD_POOL_DISPATCH_SUCCESS	分发成功
		PM_THREAD_POOL_DISPATCH_RUNNING 此任务正在跑
		PM_THREAD_POOL_DISPATCH_ERROR	有错应该终止(比如hStopEvent被置,或者发生其它严重错误)
		PM_THREAD_POOL_DISPATCH_BUSY	任务太多,不能接受新任务了
	\remark
		当不能分发任务了，返回PM_THREAD_POOL_DISPATCH_BUSY
		本函数不会抛出异常
		本函数异步操作,立即返回
*/
	virtual int tryDispatchTask( void* pdata, int datalen ) = 0;
/**	等待所有的线程终止,
	调用此函数之前,应该先设置stop事件
	timeout为超时,单位秒
	返回0表示所有线程都结束了,非0表示还有几个线程在跑
*/
	virtual int waitAll( int timeout = PM_INFINITE ) = 0;
};
typedef PmComSmartPtr<IPmThreadPool,IID_IPmThreadPool> PmThreadPoolPtr;
/** @} */ // end of group

#define PM_PROCESS_DEAD			0x01
#define PM_PROCESS_RUNNING		0x02
#define PM_PROCESS_WAIT_ERROR	0xffff

struct IPmProcess : IPmUnknown
{
	virtual PM_DWORD getId() = 0;
	virtual PM_HANDLE getHandle() = 0;
	virtual int wait( int nTimeOut = PM_INFINITE ) = 0;
	virtual int term( int nRetCode ) = 0;
};
typedef PmComSmartPtr<IPmProcess,IID_IPmProcess> PmProcessPtr;

struct IPmProcessEx : IPmProcess
{
	virtual const char* getCommandLine() = 0;
	virtual int getExitCode()=0;

	virtual void getStdout( PmString& str )=0;
	virtual void getStderr( PmString& str )=0;

	virtual const char* getStdinFileName()=0;
	virtual const char* getStdoutFileName()=0;
	virtual const char* getStderrFileName()=0;
	
	virtual PmOutputStreamPtr getOutputStream()=0;
	virtual PmOutputStreamPtr getErrorStream()=0;
	virtual PmInputStreamPtr getInputStream()=0;

	virtual PM_VOID NotKillInDestruct()=0;//吴均平添加，控制是否在进程句柄析构时杀死进程。默认杀死
};
typedef PmComSmartPtr<IPmProcessEx,IID_IPmProcessEx> PmProcessExPtr;


struct IPmStringList;
typedef PmComSmartPtr<IPmStringList,IID_IPmStringList> PmStringListPtr;

struct IPmStringList : IPmUnknown
{
	virtual int count() = 0;
	virtual PmString& getAt( int index ) = 0;
	virtual void pushBack( const PmString& str ) = 0;
	virtual void insert( int index, const PmString& str ) = 0;
	virtual void remove( int index ) = 0;
	virtual PmStringListPtr clone() = 0;
	virtual void clear() = 0;
};

struct IPmMapStringToString;
typedef PmComSmartPtr<IPmMapStringToString,IID_IPmMapStringToString> PmMapStringToStringPtr;

struct IPmMapStringToString : IPmUnknown
{
	virtual int count() = 0;
	virtual void rewind() = 0;
	virtual PM_BOOL getNext( PmString& strKey, PmString& strValue ) = 0;
	virtual PmString getAt( const char* pszKey ) = 0;
	virtual void setAt( const char* pszKey, PmString strValue ) = 0;
	virtual PM_BOOL exist( const char* pszKey ) = 0;
	virtual void remove( const char* pszKey ) = 0;
	virtual PmMapStringToStringPtr clone() = 0;
	virtual void clear() = 0;
};


#endif
