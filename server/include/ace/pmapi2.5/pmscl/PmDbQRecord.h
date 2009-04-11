#ifndef __DATABASE__QUEUE__RECORD__OBJECT__20060815
#define __DATABASE__QUEUE__RECORD__OBJECT__20060815
/**
 *	Use database to simulate a global message queue
 */
struct PmDbQRecordInfo;

/**	\brief	The PmDbQRecord class

*/
class PmDbQRecord : public PmMqMessage
{	
protected:
	PmDbQRecordInfo *m_pRecord;

public:
	enum KEY_FIELD_TYPE{MSG_ID, MSG_TYPE, MSG_TIME, MSG_STATE};

	PmDbQRecord();
	virtual ~PmDbQRecord();
	PM_INT getUserColumnCount();
	PM_LPCSTR getUserColumnName(int index) const;

	PM_LPCSTR getSystemColumnName(KEY_FIELD_TYPE type) const ;
	PM_VOID setSystemColumnName(KEY_FIELD_TYPE type, PM_LPCSTR szColumnName);

	/**	\brief	add a user costumized column
			
		\param	colname	column name
		\param	ptrVal	variable pointer used to hold column value

		\return	PM_BOOL

		
	*/
	PM_VOID addUserColumn(PM_LPCSTR colname, PmVariantPtr ptrVal);

	PmDbColumn& getUserColumn(PM_ULONG index);
};



#endif
