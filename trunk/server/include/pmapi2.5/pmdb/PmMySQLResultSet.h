#ifndef __PM__MYSQL__RESULTSET__20040424
#define __PM__MYSQL__RESULTSET__20040424

/**	\brief	The PmMysqlResultSet class

*/
class PmMysqlResultSet : public IPmResultSet
{
public:
	PMCOM_BEGIN_MAP(IPmResultSet)
		PMCOM_CHAIN_INTERFACE( IPmResultSet )
	PMCOM_END_MAP

	PmMysqlResultSet(MYSQL_RES *);
	virtual ~PmMysqlResultSet();

	virtual PM_ULONG getRowCount();
	virtual PM_BOOL fetchRow();
	virtual const char *getAt(int iIndex);
	virtual const char *operator [](int iIndex);
	virtual PM_ULONG getColumnCount();
protected:
	MYSQL_RES *m_res;
	MYSQL_ROW m_row;
};
#endif


