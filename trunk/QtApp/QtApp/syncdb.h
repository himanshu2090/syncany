#ifndef SYNCQUEUE_H
#define SYNCQUEUE_H

#include "common.h"

//接收和发送队列，构造为一个单体类来提供数据持久化支持


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
	void put_cmd(QString strCmdID,QString strCmdStr);
	void tag_cmd(QString strCmdID,int tag);
	void tag_cmd(QString strCmdID,int tag,QString strCmdRet);
	void execSql(QString strSql);

private:
	int m_type;
	QString m_strTable;
	QString m_dbFile;
	CppSQLite3DB m_db;
};

#endif // SYNCQUEUE_H
