#ifndef __PM_SCL_INTERFACE_DEF_H__
#define __PM_SCL_INTERFACE_DEF_H__

/**	
 *@file PmInterfaceDef.h 
 *@brief pmapi�ӿڶ���
*/
/** @defgroup pmapi_interface pmapi���нӿڶ���
 *  ����pmapi�Ľӿڶ���
 *  @{
 */

/**	\brief mysql���ݿ�ӿڵ���������
*/
#define CLSID_PM_DBCONN_MYSQL "CLSID_PM_DBCONN_MYSQL"

/**	\brief ���ݿ����ӵĽӿ�ID
*/
#define IID_IPmDbConnection		0x10000002
/**	\brief ���ݿ���ʽ�����ϵĽӿ�ID
*/
#define IID_IPmResultSet		0x10000003
/**	\brief ��Ϣ���нӿ�ID
*/
#define IID_IPmMessageQueue		0x10000004
/**	\brief �̵߳Ľӿ�ID
*/
#define IID_IPmThread			0x10000005
/**	\brief ����Ľӿ�ID
*/
#define IID_IPmService			0x10000006
/**	\brief �쳣�Ľӿ�ID
*/
#define IID_IPmException		0x10000007
/**	\brief ͬ������Ľӿ�ID
*/
#define IID_IPmSynchronizer		0x10000008
/**	\brief �������Ľӿ�ID
*/
#define IID_IPmInputStream		0x10000009
/**	\brief ����������Ľӿ�ID
*/
#define IID_IPmInputStreamable	0x1000000a
/**	\brief ����������Ľӿ�ID
*/
#define IID_IPmCondition		0x1000000b
/**	\brief �����б�Ľӿ�ID
*/
#define IID_IPmProperties		0x1000000c
/**	\brief ������Ľӿ�ID
*/
#define IID_IPmInspector		0x1000000d
/**	\brief ������Ľӿ�ID
*/
#define IID_IPmOutputStream		0x1000000e
/**	\brief ���������Ľӿ�ID
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

/**	\brief	���ݿ����������ӿ�
	\note	���ݿ������������ʱͨ���˽ӿ�,�˽ӿڵ�ʵ������ͨ��IPmDbConnection::query�����ɵ�,
			��������������ݿ��һ����¼,����ͨ��getAt����operator []�����ĳ�е�ֵ��
			������������з����˴����׳�PmException�쳣
	\code �������ݿ����ʾ��

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
/**	\brief	The getRowCount function ������м�¼������
	\return	PM_ULONG ��¼����
*/
	virtual PM_ULONG getRowCount()=0;
/**	\brief	The fetchRow function �����һ�м�¼
	\return	PM_BOOL �����Ƿ��Ѿ����м�¼�������PM_FALSE��ʾ�Ѿ���βû�и����¼�������У�������ݷ��ʴ����׳�PmException�쳣
*/
	virtual PM_BOOL fetchRow()=0;
/**	\brief	The getAt function ��õ�ǰ��ĳ�е����ݣ����ȱ���ɹ�����fetchRow
	\param[in]	iIndex	a parameter of type int ����ţ�0��ʼ�������߱����Լ���֤��Խ��
	\return	const char* ���س����ַ���ָ��
*/
	virtual const char *getAt(int iIndex)=0;
/**	\brief	The [] function ��õ�ǰ��ĳ�е����ݣ����ȱ���ɹ�����fetchRow
	\param[in]	iIndex	a parameter of type int ����ţ�0��ʼ�������߱����Լ���֤��Խ��
	\return	const char *operator
*/
	virtual const char *operator [](int iIndex)=0;
/**	\brief	The getColumnCount function ����е���Ŀ
	\return	PM_ULONG ����
*/
	virtual PM_ULONG getColumnCount() = 0;
};
/**	\brief	Redefinition of PmComSmartPtr<IPmResultSet,IID_IPmResultSet> as PmResultSetPtr
*/
typedef PmComSmartPtr<IPmResultSet,IID_IPmResultSet> PmResultSetPtr;


/**	\brief	���ݿ����ӽӿ�

*/
struct IPmDbConnection : IPmUnknown
{
/**	\brief	The connect function
	\param	pUri a parameter of type const PmUri*, ͨ����uri������ӵ���Ϣ uri����mysql://root:pwd\@localhost/cef_push
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
	\param	pUri a parameter of type const PmUri*, ͨ����uri������ӵ���Ϣ uri����mysql://root:pwd\@localhost/cef_push
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

/**	\brief	��Ϣ���нӿ�

*/
struct IPmMessageQueue : IPmUnknown
{
	virtual PM_INT get(PmMqMessage& pMsg) = 0;
	virtual PM_INT put(PmMqMessage& pMsg) = 0;
};
/**	\brief	Redefinition of PmComSmartPtr<IPmMessageQueue,IID_IPmMessageQueue> as PmMessageQueuePtr
*/
typedef PmComSmartPtr<IPmMessageQueue,IID_IPmMessageQueue> PmMessageQueuePtr;


/**	\brief �߳̽ӿ�

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


/**	\brief	ͬ���ӿ�

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

/**	\brief	ͬ�������ӿ�

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


/**	\brief	����ӿ�

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

/**	\brief	�������к���

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

/**	\brief	The �쳣�ӿ�

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

/**	\brief	������ӿ�

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

/**	\brief	The IPmProperties struct �����б��ܸ��������������úͻ�ȡ����ֵ
	\note	����Ϊ���ֶԷ�ʽ,����=>ֵ, ���ƺ�ֵ���������ַ�����Ϣ��
			���Ʋ�Ҫ��������,ǰ��Ҫ�пո�
			���ƴ�Сд��ء�
*/
struct IPmProperties : IPmUnknown
{
	/**	\brief	��������ֵ
		\param[in]	pszProtertyName	��������(Ҫ����Ψһ��)
		\param[in]	pmstrPropertyValue ����ֵ	
		\return	void
		*/
	virtual void setProperty( PM_LPCSTR pszProtertyName, PmString pmstrPropertyValue )=0;

	/**	\brief	��������������ȡ����ֵ
		\param[in]	pszProtertyName	��������
		\return PmString ����������Ӧ������ֵ
				��������Ƶ����Բ����ڣ�Ӧ���׳�PmException�쳣,������PM_ERROR_PROPERTY_NOTEXIST
	*/
	virtual PM_LPCSTR getProperty( PM_LPCSTR pszProtertyName )=0;

	/**	\brief 	���ñ������Ե��α�ָ��
		\note	��һ�ε���getNextProperty����ǰ,Ҫ�ȵ��øú����������п���ȡ��ȫ��������
		\return	void		
	*/
	virtual void rewindProperties()=0;

	/**	\brief	��ȡ��һ������ֵ
		\param[out]	strName		��������
		\param[out]	strValue	����ֵ
		\return	PM_BOOL:ȡ����������ֵ�Է���PM_TRUE;���˽�β����PM_FALSE	
	*/
	virtual PM_BOOL getNextProperty( PmString& strName, PmString& strValue )=0;
};
/**	\brief	Redefinition of PmComSmartPtr<IPmProperties, IID_IPmProperties> as PmPropertiesPtr IPmProperties������ָ��
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
		�Ӷ�����ȡ�õĶ��г�Ա������
	\param	datalen	a parameter of type int
		�Ӷ�����ȡ�õĶ��г�Ա�����ݳ���
	\param	hStopEvent	a parameter of type PM_HANDLE
		�˳��¼�,�������Ժ󱾺���Ӧ�þ����˳�
	\param	dwUserData	a parameter of type PM_DWORD
		�û�����,initThreadPoolʱ����Ĳ���
	\return	typedef int
		����0��ʾ����ɹ�,����-1��ʾ�����з������ش���,Ӧ���˳�,��������Ӧ���׳��쳣
	\remark ����������������ص�����

*/

typedef int (*PF_PM_TASK_HANDLER)( void* pdata, int datalen, PM_HANDLE hStopEvent, PM_DWORD dwUserData );

#define PM_THREAD_POOL_DISPATCH_SUCCESS	0
#define PM_THREAD_POOL_DISPATCH_BUSY	2
#define PM_THREAD_POOL_DISPATCH_ERROR	-1

struct IPmThreadPool : IPmUnknown
{
/**	\brief	The initThreadPool function

	\param	nMaxThreadCount	a parameter of type int
		�̳߳ص��߳�����
	\param	pfTaskHandler	a parameter of type PF_PM_TASK_HANDLER
		����ص�����
	\param	hStopEvent	a parameter of type PM_HANDLE
		ֹͣ�¼�
	\param	dwUserData	a parameter of type PM_DWORD
		�û�����
	\return	virtual void
	\remark �뱣֤�˺�����dispatchTask֮ǰִ��,������dispatchTask֮�󲻱�ִ��,���Υ��,���׳��쳣
		ִ�б�����ʱ,�Ὺ��nMaxThreadCount���߳�,�ȴ�����ַ�
		���ַ�һ�������,ĳһ���̻߳����������,�ص����������߳��лص���
		�ڻص�������,�������hStopEvent����,Ӧ�����˳�
*/

	virtual void initThreadPool( int nMaxThreadCount, PF_PM_TASK_HANDLER pfTaskHandler, PM_HANDLE hStopEvent, PM_DWORD dwUserData ) = 0;
/**	\brief	The dispatchTask function
	\param	pdata	a parameter of type void*
		�ַ��������������Ϣ,����Ӧ��С��һ��������Ϣ�ܹ����ܵĴ�С(PM_MQ_MESSAGE_MAXLEN-64)
	\param	datalen	a parameter of type int
		�ַ��������������Ϣ�ĳ���
	\return	int
		���������¼��ַ���ֵ
		PM_THREAD_POOL_DISPATCH_SUCCESS	�ַ��ɹ�
		PM_THREAD_POOL_DISPATCH_RUNNING ������������
		PM_THREAD_POOL_DISPATCH_ERROR	�д�Ӧ����ֹ(����hStopEvent����,���߷����������ش���)
	\remark
		�����ַܷ������ˣ�����������ȴ���һֱ�ȵ��п��л���hStopEvent����
		�����������׳��쳣
*/
	virtual int dispatchTask( void* pdata, int datalen ) = 0;
/**	\brief	The tryDispatchTask function
	\param	pdata	a parameter of type void*
		�ַ��������������Ϣ,����Ӧ��С��һ��������Ϣ�ܹ����ܵĴ�С(PM_MQ_MESSAGE_MAXLEN-64)
	\param	datalen	a parameter of type int
		�ַ��������������Ϣ�ĳ���
	\return	virtual int
		PM_THREAD_POOL_DISPATCH_SUCCESS	�ַ��ɹ�
		PM_THREAD_POOL_DISPATCH_RUNNING ������������
		PM_THREAD_POOL_DISPATCH_ERROR	�д�Ӧ����ֹ(����hStopEvent����,���߷����������ش���)
		PM_THREAD_POOL_DISPATCH_BUSY	����̫��,���ܽ�����������
	\remark
		�����ַܷ������ˣ�����PM_THREAD_POOL_DISPATCH_BUSY
		�����������׳��쳣
		�������첽����,��������
*/
	virtual int tryDispatchTask( void* pdata, int datalen ) = 0;
/**	�ȴ����е��߳���ֹ,
	���ô˺���֮ǰ,Ӧ��������stop�¼�
	timeoutΪ��ʱ,��λ��
	����0��ʾ�����̶߳�������,��0��ʾ���м����߳�����
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

	virtual PM_VOID NotKillInDestruct()=0;//���ƽ��ӣ������Ƿ��ڽ��̾������ʱɱ�����̡�Ĭ��ɱ��
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
