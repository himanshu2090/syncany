#ifndef __PM_SCL_PM_DB_QUEUE_H__
#define __PM_SCL_PM_DB_QUEUE_H__

/**	\brief	The PmDbQueue// class 数据库队列
	使用本数据库队列时，必须使用后立即释放队列类，不要保留数据库连接，否则在连接池情况下可能导致不可预知的错误
	例如
	PmDbQueue dbq;//不要把此变量设置为类成员变量或者全局变量
	dbq.setDbConnection( getCefResource()->getDbConnection() );
	dbq.open( "x table");
	dbq.getex();
	dbq.putex();
	//自动释放
	//2006 09 05 刘宾

*/


// Interface of Global Message Queue Client
class PmDbQueue// : public IMessageQueue
{
protected:
	void replaceSql( PmDbQRecord& rec, PmString& strSql);
	void setLastMsgRead();


	PmString		m_strTableName;
	PM_ULONG		m_ulIdLastGet;
	PmDbConnectionPtr m_ptrDbConnection;

	PmString		m_strStateName;
	PmString		m_strIdName;

	void generateSql( PmString& strSql );
//
//	/* close the current connection and open again */
////	PM_BOOL Reopen();
//
public:
	PmDbQueue();
	virtual ~PmDbQueue();

	void setDbConnection(PmDbConnectionPtr ptr);
    /*
    *	Open a table, set the table name as a matter of fact
    */ 
    virtual void open(const char *tbname);

	/*
	 *	close the table, do cleanup job
	 */ 
	virtual PM_VOID close();

	/*
	 *  Put a message in queue
	 *  iMode: 0 unblock, 1 block
	 */
//	virtual PM_VOID put(PmMqMessage& gmdata, int iMode=0);

	/*
	 *  Get a message from queue, Message's status will be set as USED, 
	 *  next call to Get will get next message. If program crashes or 
	 *  any reason cause PM_Queue quit without committing, all message 
	 *  will be rolled back to status new
	 *  iMode: 0 unblock, 1 block	
	 */
//	virtual PM_VOID get(PmMqMessage& gmdata, int iMode=0);
	
	/*
	 *	Find specific data record with unique data id equals ulDataID
	 */ 
//	virtual PM_BOOL find(PM_ULONG ulDataID, PmMqMessage& rec);
//	virtual PM_BOOL findEx(PM_ULONG ulDataID, PmDbQRecord& rec);

	/*
	 *  Remove the data record from the queue
	 */
//	virtual PM_VOID Remove(PmMqMessage& gmdata, int iMode=0);

//	virtual PM_VOID setMsgState(PmMqMessage& gmdata, char state);

	/*
	 *  Put a customizable queue message in database
	 *  iMode: 0 unblock, 1 block
	 */ 
	virtual PM_VOID putEx(PmDbQRecord& gmdata, int iMode=0);

	/*
	 *  Get a customizable queue message from database
	 *  iMode: 0 unblock, 1 block
	 */
	virtual PM_VOID getEx(PmDbQRecord& gmdata, int iMode=0);
	
	//获得时间戳至少已经打了nSeconds秒的记录,如果为0,时间戳参数忽略
	virtual PM_VOID getWithTime(PmDbQRecord& gmdata, int nSeconds, int iMode=0);
};


class PmDbQueueEx :  public PmDbQueue
{
public:
	PmDbQueueEx();
	virtual ~PmDbQueueEx();
	void deleteLastMsgRead();
	virtual PM_VOID getWithTime(PmDbQRecord& gmdata, int nSeconds, int iMode=0);
	virtual PM_VOID close();
};

#endif


