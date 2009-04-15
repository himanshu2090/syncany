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

//命令队列
/*
自增长编号(id)
命令编号(cmd_id)
标志(tag)  0未发送/1已经发送/2收到响应 
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

void SyncDB::createTable() //在构造函数里调用，未加锁，因为在创建单体实例前已经加锁
{
	QString strTableName=strQueueTableName[QUEUE_IN];
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
	strTableName=strQueueTableName[QUEUE_OUT];
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
	//在这里添加其他需要初始化创建的表

}


int SyncDB::put_cmd(QString strCmdID,QString strCmdStr,QUEUE_ID nQueue)
{
	QString strSql="insert into [%1] (cmd_id,tag,cmd_str,create_time) values (%2,%3,'%4',date('now'))";
	return execSql(strSql.arg(strQueueTableName[nQueue]).arg(strCmdID).arg(TAG_UNSEND).arg(strCmdStr));
}

int SyncDB::tag_cmd(QString strCmdID,int tag,QUEUE_ID nQueue)
{
	QString strSql="update [%1] set tag=%2 where cmd_id=%3";
	return execSql(strSql.arg(strQueueTableName[nQueue]).arg(tag).arg(strCmdID));
}

int SyncDB::tag_cmd(QString strCmdID,int tag,QString strCmdRet,QUEUE_ID nQueue)
{
	QString strSql="update [%1] set tag=%2,cmd_ret='%3' where cmd_id=%4";
	return execSql(strSql.arg(strQueueTableName[nQueue]).arg(tag).arg(strCmdRet).arg(strCmdID));
}

bool SyncDB::exist_cmd(QString strCmdID,QUEUE_ID nQueue)
{
	QString strSql="select count(*) as cnt from %1 where cmd_id=%2";
	CppSQLite3Query result=querySql(strSql.arg(strQueueTableName[nQueue]).arg(strCmdID));
	if(!result.eof())
	{
		if(!result.fieldIsNull(0))
		{
			return (QString(result.getStringField(0)).toInt() > 0);
		}
	}
	return false;
}


CommandMap SyncDB::get_cmd(int tag,QUEUE_ID nQueue)
{
	CommandMap props;
	QString strSql;
	strSql="select cmd_str from %1 where tag=%2 limit 1";
	CppSQLite3Query result=querySql(strSql.arg(strQueueTableName[nQueue]).arg(tag));
	if(!result.eof())
	{
		if(!result.fieldIsNull(0))
		{
			props=convert_from_cmdline(result.getStringField(0));
		}
	}
	return props;
}

CommandMap SyncDB::get_cmd(QString strCmdID,QUEUE_ID nQueue)
{
	CommandMap props;
	QString strSql;
	strSql="select cmd_str from %1 where cmd_id=%2 limit 1";
	CppSQLite3Query result=querySql(strSql.arg(strQueueTableName[nQueue]).arg(strCmdID));
	if(!result.eof())
	{
		if(!result.fieldIsNull(0))
		{
			props=convert_from_cmdline(result.getStringField(0));
		}
	}
	return props;
}


int SyncDB::reset_cmd_queue()
{
	QString strSql;
	strSql="update [%1] set tag=%2 where tag=%3 and cmd_str like '%4%%'";
	execSql(strSql.arg(strQueueTableName[QUEUE_OUT]).arg(TAG_ABANDON).arg(TAG_SENDING).arg(get_cmdstr(CMD_HELLO)));
	strSql="update [%1] set tag=%2 where tag=%3";
	int ret = execSql(strSql.arg(strQueueTableName[QUEUE_OUT]).arg(TAG_UNSEND).arg(TAG_SENDING)) +
	execSql(strSql.arg(strQueueTableName[QUEUE_IN]).arg(TAG_UNSEND).arg(TAG_SENDING));
	return ret;
}


int SyncDB::execSql(QString strSql)
{
	return execSql(strSql.toLocal8Bit());
}

int SyncDB::execSql(QByteArray strSql)
{
	QMutexLocker locker(&g_locker_db);
	return m_db.execDML(strSql.data());
}

CppSQLite3Query  SyncDB::querySql(QByteArray strSql)
{
	QMutexLocker locker(&g_locker_db);
	return m_db.execQuery(strSql.data());
}

CppSQLite3Query  SyncDB::querySql(QString strSql)
{
	QMutexLocker locker(&g_locker_db);
	return m_db.execQuery(strSql.toLocal8Bit().data());
}

CommandMap SyncDB::singleQuerySql(QString strSql)
{
	CppSQLite3Query result=querySql(strSql);
	CommandMap props;
	if(!result.eof())
	{
		for(int i=0;i<result.numFields();++i)
		{
			props[result.fieldName(i)]=result.fieldValue(i);
		}
	}
	return props;
}



