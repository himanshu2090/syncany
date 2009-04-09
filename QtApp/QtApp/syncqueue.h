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

//�������
/*
���������(id)
������(cmd_id)
��־(tag)  0δ����/1���/2���� 
��������(cmd_str)
��Ӧͷ��������(cmd_ret)
���ʼʱ��(begin_time)
�����Ӧʱ��(end_time)

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
