#include "syncdb.h"
#include "synconf.h"

SyncDB * SyncDB::m_instance=null;
QMutex g_locker_db;

SyncDB::SyncDB(QObject *parent)
	: QObject(parent)
{
	m_dbFile=Synconf::instance()->getstr("work_dir")+"syncdb.db3";
	m_db.open(m_dbFile.toStdString().c_str());
	createTable();
}

SyncDB::~SyncDB()
{

}

SyncDB *SyncDB::instance()
{
	QMutexLocker locker(&g_locker_db);
	if(m_instance==null)
	{
		m_instance=new SyncDB(0);
	}
	return m_instance;
}

//�������
/*
���������(id)
������(cmd_id)
��־(tag)  0δ����/1�Ѿ�����/2�յ���Ӧ 
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
[create_time] DATETIME,
[send_time] DATETIME, 
CONSTRAINT [sqlite_autoindex_sqin_1] PRIMARY KEY ([cmd_id]));

CREATE TABLE [sqout] (
[id] AUTOINC, 
[cmd_id] INT NOT NULL, 
[tag] INT NOT NULL DEFAULT 0, 
[cmd_str] VARCHAR NOT NULL, 
[cmd_ret] VARCHAR, 
[create_time] DATETIME,
[send_time] DATETIME, 
[end_time] DATETIME, 
CONSTRAINT [sqlite_autoindex_sqout_1] PRIMARY KEY ([cmd_id]));
*/

void SyncDB::createTable() //�ڹ��캯������ã�δ��������Ϊ�ڴ�������ʵ��ǰ�Ѿ�����
{
	QString strTableName="sqin";
	if(!m_db.tableExists(strTableName.toStdString().c_str()))
	{
		QString strSql;
		strSql+="CREATE TABLE [";
		strSql+=strTableName+"] (\n"
			"[id] AUTOINC, \n"
			"[cmd_id] INT NOT NULL, \n"
			"[tag] INT NOT NULL DEFAULT 0, \n"
			"[cmd_str] VARCHAR NOT NULL, \n"
			"[cmd_ret] VARCHAR, \n"
			"[create_time] DATETIME,\n"
			"[send_time] DATETIME, \n"
			"[end_time] DATETIME, \n"
			"CONSTRAINT [sqlite_autoindex_%1_1] PRIMARY KEY ([cmd_id]));\n";
		m_db.execDML(strSql.toStdString().c_str());
	}
	strTableName="sqout";
	if(!m_db.tableExists(strTableName.toStdString().c_str()))
	{
		QString strSql;
		strSql+="CREATE TABLE [";
		strSql+=strTableName+"] (\n"
			"[id] AUTOINC, \n"
			"[cmd_id] INT NOT NULL, \n"
			"[tag] INT NOT NULL DEFAULT 0, \n"
			"[cmd_str] VARCHAR NOT NULL, \n"
			"[cmd_ret] VARCHAR, \n"
			"[create_time] DATETIME,\n"
			"[send_time] DATETIME, \n"
			"[end_time] DATETIME, \n"
			"CONSTRAINT [sqlite_autoindex_%1_1] PRIMARY KEY ([cmd_id]));\n";
		m_db.execDML(strSql.toStdString().c_str());
	}
	//���������������Ҫ��ʼ�������ı�

}


void SyncDB::put_cmd(QString strCmdID,QString strCmdStr)
{
	QString strSql="insert into [%1] (cmd_id,tag,cmd_str,create_time) values (%2,0,'%3',date('now'))";
	strSql.arg(strCmdID,strCmdStr);
	m_db.execDML(strSql.toStdString().c_str());
}

void SyncDB::tag_cmd(QString strCmdID,int tag)
{
}

void SyncDB::tag_cmd(QString strCmdID,int tag,QString strCmdRet)
{
}

