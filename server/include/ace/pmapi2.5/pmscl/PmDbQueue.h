#ifndef __PM_SCL_PM_DB_QUEUE_H__
#define __PM_SCL_PM_DB_QUEUE_H__

/**	\brief	The PmDbQueue// class ���ݿ����
	ʹ�ñ����ݿ����ʱ������ʹ�ú������ͷŶ����࣬��Ҫ�������ݿ����ӣ����������ӳ�����¿��ܵ��²���Ԥ֪�Ĵ���
	����
	PmDbQueue dbq;//��Ҫ�Ѵ˱�������Ϊ���Ա��������ȫ�ֱ���
	dbq.setDbConnection( getCefResource()->getDbConnection() );
	dbq.open( "x table");
	dbq.getex();
	dbq.putex();
	//�Զ��ͷ�
	//2006 09 05 ����

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
	
	//���ʱ��������Ѿ�����nSeconds��ļ�¼,���Ϊ0,ʱ�����������
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


