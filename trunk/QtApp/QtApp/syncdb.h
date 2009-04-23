#ifndef SYNCQUEUE_H
#define SYNCQUEUE_H

#include "common.h"

//接收和发送队列，构造为一个单体类来提供数据持久化支持
//命令队列ID
enum QUEUE_ID
{
	QUEUE_IN=0,
	QUEUE_OUT=1,
};

static const char* strQueueTableName[]=
{
	"sqin",
	"sqout"
};

enum SYNC_TYPES
{
	SYNC_FILES=0,
};

static const char* strSyncTableName[]=
{
	"sync_files",
};

class SyncDB : public QObject
{
	Q_OBJECT
private:
	SyncDB(QObject *parent=0);
public:
	~SyncDB();
	
private:
	void createTable();
public:
	static SyncDB *m_instance;
	static SyncDB *instance();//返回不同的命令单体队列

public slots:
	//命令相关的槽接口
	int cmd_put(QString strCmdID,QString strCmdLine,QUEUE_ID nQueue=QUEUE_IN);
	int cmd_tag(QString strCmdID,int tag,QString strCmdRet=QString(""),QUEUE_ID nQueue=QUEUE_IN);
	bool cmd_exist(QString strCmdID,QUEUE_ID nQueue=QUEUE_IN);
	int cmd_reset_queue();//将所有未完成的命令转为未发送状态，在程序退出或启动时执行，以便重发命令
	CommandMap cmd_get(QString strCmdID,QUEUE_ID nQueue=QUEUE_IN);

	//可同步的SQL语句执行接口
	int execSql(QString strSql);
	int execSql(QByteArray strSql);
	CppSQLite3Query querySql(QByteArray strSql);
	CppSQLite3Query querySql(QString strSql);
	CommandMap singleQuerySql(QString strSql);
	bool tableExists(QString);
private:
	int m_type;
	QString m_strTable;
	QString m_dbFile;
	CppSQLite3DB m_db;
};

#endif // SYNCQUEUE_H
