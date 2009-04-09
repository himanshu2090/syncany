#ifndef SYNCQUEUE_H
#define SYNCQUEUE_H

#include <QObject>
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
		QString strSql;
		strSql.sprintf("CREATE TABLE [%s] (\n"
			"[id] AUTOINC, \n"
			"[cmd_id] INT NOT NULL, \n"
			"[tag] INT NOT NULL DEFAULT 0, \n"
			"[cmd_str] VARCHAR NOT NULL, \n"
			"[cmd_ret] VARCHAR, \n"
			"[begin_time] DATETIME, \n"
			"[end_time] DATETIME, \n"
			"CONSTRAINT [sqlite_autoindex_sqin_1] PRIMARY KEY ([cmd_id]));\n",(m_type==0)?"sqin":"sqout");

	}

public slots:

	

private:
	int m_type;

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
