#ifndef SYNCQUEUE_H
#define SYNCQUEUE_H

#include <QtCore/QtCore>
#include "CppSQLite3.h"

class SyncQueue : public QObject
{
	Q_OBJECT

public:
	SyncQueue(QObject *parent=0,int type=0);
	~SyncQueue();
	
private:
	void createTable()
	{
		QMutexLocker locker(&m_locker);
		QString strTableName=(m_type==0)?"sqin":"sqout";
		if(m_db.tableExists(strTableName.toStdString().c_str()))
			return ;
		QString strSql="CREATE TABLE [%1] (\n"
			"[id] AUTOINC, \n"
			"[cmd_id] INT NOT NULL, \n"
			"[tag] INT NOT NULL DEFAULT 0, \n"
			"[cmd_str] VARCHAR NOT NULL, \n"
			"[cmd_ret] VARCHAR, \n"
			"[begin_time] DATETIME, \n"
			"[end_time] DATETIME, \n"
			"CONSTRAINT [sqlite_autoindex_%1_1] PRIMARY KEY ([cmd_id]));\n";
		strSql.arg(strTableName);
		m_db.execDML(strSql.toStdString().c_str());
	}

public slots:

	

private:
	int m_type;
	QMutex m_locker;
	CppSQLite3DB m_db;
};

//命令队列
/*
自增长编号(id)
命令编号(cmd_id)
标志(tag)  0未发送/1完成/2放弃 
命令内容(cmd_str)
响应头，无数据(cmd_ret)
命令开始时间(begin_time)
命令回应时间(end_time)

CREATE TABLE [sqin] (
[id] AUTOINC, 
[cmd_id] INT NOT NULL, 
[tag] INT NOT NULL DEFAULT 0, 
[cmd_str] VARCHAR NOT NULL, 
[cmd_ret] VARCHAR, 
[begin_time] DATETIME, 
[end_time] DATETIME, 
CONSTRAINT [sqlite_autoindex_sqin_1] PRIMARY KEY ([cmd_id]));

CREATE TABLE [sqout] (
[id] AUTOINC, 
[cmd_id] INT NOT NULL, 
[tag] INT NOT NULL DEFAULT 0, 
[cmd_str] VARCHAR NOT NULL, 
[cmd_ret] VARCHAR, 
[begin_time] DATETIME, 
[end_time] DATETIME, 
CONSTRAINT [sqlite_autoindex_sqout_1] PRIMARY KEY ([cmd_id]));
*/

#endif // SYNCQUEUE_H
