#ifndef __PM_DB_H__
#define __PM_DB_H__

//#define PM_IDBCONNFAC_MYSQL_NAME "IPmDbConnFactory_mysql"
//#define PM_IDBCONNFAC_ORACLE_NAME "IPmDbConnFactory_oracle"
//#define PM_IDBCONNFAC_SQLSERVER_NAME "IPmDbConnFactory_sqlserver"

//#define PM_IDBMANAGER "IPmDbManage"

//#define CLSID_PM_DBCONN_MYSQL "CLSID_PM_DBCONN_MYSQL"


//struct IPmResultSet : IPmUnknown
//{
//	virtual PM_ULONG getRowCount()=0;
//	virtual PM_BOOL fetchRow()=0;
//	virtual const char *getAt(int iIndex)=0;
//	virtual const char *operator [](int iIndex)=0;
//	virtual PM_ULONG getColumnCount()=0;
//};
//typedef PmComSmartPtr<IPmResultSet,PmIID(IID_IPmDbManager)> PmResultSetPtr;
//struct IPmDbConnection : IPmUnknown
//{
//	virtual void connect(const PmUri*)=0;
//	virtual void disconnect()=0;
//	virtual IPmResultSet* query(PM_LPCSTR sqlstmt)=0;
//	virtual void execute(PM_LPCSTR sqlstmt)=0;
//
////	virtual PM_BOOL EscapeString(ByteStr *data)=0;
//};
//typedef PmComSmartPtr<IPmDbConnection> PmDbConnectionPtr;
//struct IPmDbManager : IPmUnknown
//{
//	virtual IPmDbConnection* getConnection(const PmUri*)=0;
//};
//typedef PmComSmartPtr<IPmDbManager> PmDbManagerPtr;
//

#endif
