#ifndef __MYSQL_DATABASE__CONNECTION__20040424
#define __MYSQL_DATABASE__CONNECTION__20040424

class PmMysqlDbConnection : public IPmDbConnection, public IPmDbConnectionEx
{
public:
	PmMysqlDbConnection();
	virtual ~PmMysqlDbConnection();
	PMCOM_BEGIN_MAP(IPmDbConnection)
		PMCOM_CHAIN_INTERFACE( IPmDbConnection )
		PMCOM_CHAIN_INTERFACE( IPmDbConnectionEx )
	PMCOM_END_MAP

	virtual void connect(const PmUri*);
	virtual void disconnect();
	virtual PmResultSetPtr query(PM_LPCSTR sqlstmt);
	virtual void execute(PM_LPCSTR sqlstmt);

	virtual void connectEx(const PmUri* pUri, const char* pszFile, int nFileLine );
	virtual void disconnectEx(const char* pszFile, int nFileLine );
	virtual PmResultSetPtr queryEx(PM_LPCSTR sqlstmt, const char* pszFile, int nFileLine );
	virtual void executeEx(PM_LPCSTR sqlstmt, const char* pszFile, int nFileLine );

protected:
	void connectReal(const PmUri* pUri, const char* pszFile, int nFileLine );
	void disconnectReal(const char* pszFile, int nFileLine );
	PmResultSetPtr queryReal(PM_LPCSTR sqlstmt, const char* pszFile, int nFileLine );
	void executeReal(PM_LPCSTR sqlstmt, const char* pszFile, int nFileLine );

	void testAndFixConnection();
	MYSQL *m_myData;
	PmUri m_uri;
	void mysql_query(const char *sql);
};

#endif

