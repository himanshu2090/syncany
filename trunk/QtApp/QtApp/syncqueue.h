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
