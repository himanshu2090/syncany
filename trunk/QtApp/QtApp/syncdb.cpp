#include "syncdb.h"
#include "synconf.h"

SyncDB * SyncDB::m_instance=null;
QMutex g_locker_db;

SyncDB::SyncDB(QObject *parent)
	: QObject(parent)
{
	m_dbFile=Synconf::instance()->getstr(KEY_WORK_DIR)+"syncdb.db3";
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
[id] AUTOINC DEFAULT 1, 
[cmd_id] INT NOT NULL, 
[tag] INT NOT NULL DEFAULT 0, 
[cmd_str] VARCHAR NOT NULL, 
[cmd_ret] VARCHAR, 
[create_time] DATETIME,
[send_time] DATETIME, 
CONSTRAINT [sqlite_autoindex_sqin_1] PRIMARY KEY ([cmd_id]));

CREATE TABLE [sqout] (
[id] AUTOINC DEFAULT 1, 
[cmd_id] INT NOT NULL, 
[tag] INT NOT NULL DEFAULT 0, 
[cmd_str] VARCHAR NOT NULL, 
[cmd_ret] VARCHAR, 
[create_time] DATETIME,
[send_time] DATETIME, 
[end_time] DATETIME, 
CONSTRAINT [sqlite_autoindex_sqout_1] PRIMARY KEY ([cmd_id]));
*/

/*
文件同步表：
filename	本地磁盘文件名（带全路径）
filesize	本地文件大小
url_local	无相对URI表示
anchor 锚点（服务器产生）
anchor_time 锚点对应的最后修改时间，本地存储，在更新锚点时更新
modify_time 最后修改时间

CREATE TABLE [sync_files] (
[fid] AUTOINC DEFAULT 1, 
[url_local] VARCHAR NOT NULL, 
[filename] VARCHAR , 
[filesize] INT , 
[anchor] INT, 
[anchor_time] DATETIME, 
[modify_time] DATETIME,
CONSTRAINT [sqlite_autoindex_sync_files_url_local] PRIMARY KEY ([url_local]));
*/

void SyncDB::createTable() //在构造函数里调用，未加锁，因为在创建单体实例前已经加锁
{
        QString strTableName=TABLE_SQIN;
	if(!m_db.tableExists(strTableName.toStdString().c_str()))
	{
		QString strSql;
		strSql+="CREATE TABLE [";
		strSql+=strTableName+"] (\n"
			"[id]  INTEGER PRIMARY KEY AUTOINCREMENT , \n"
			"[cmd_id] INT NOT NULL, \n"
			"[tag] INT NOT NULL DEFAULT 0, \n"
			"[cmd_str] VARCHAR NOT NULL, \n"
			"[cmd_ret] VARCHAR, \n"
			"[create_time] DATETIME,\n"
			"[send_time] DATETIME, \n"
			"[end_time] DATETIME);";
		m_db.execDML(strSql.toStdString().c_str());
		strSql="CREATE UNIQUE INDEX ["+strTableName+"_cmd_id] ON ["+strTableName+"] ([cmd_id]);";
		m_db.execDML(strSql.toStdString().c_str());
	}
        strTableName=TABLE_SQOUT;
	if(!m_db.tableExists(strTableName.toStdString().c_str()))
	{
		QString strSql;
		strSql+="CREATE TABLE [";
		strSql+=strTableName+"] (\n"
			"[id]  INTEGER PRIMARY KEY AUTOINCREMENT , \n"
			"[cmd_id] INT NOT NULL, \n"
			"[tag] INT NOT NULL DEFAULT 0, \n"
			"[cmd_str] VARCHAR NOT NULL, \n"
			"[cmd_ret] VARCHAR, \n"
			"[create_time] DATETIME,\n"
			"[send_time] DATETIME, \n"
			"[end_time] DATETIME);\n";
		m_db.execDML(strSql.toStdString().c_str());
		strSql="CREATE UNIQUE INDEX ["+strTableName+"_cmd_id] ON ["+strTableName+"] ([cmd_id]);";
		m_db.execDML(strSql.toStdString().c_str());
	}
	
}


int SyncDB::cmd_put(QString strCmdID,QString strCmdStr,QUEUE_ID nQueue)
{
	QString strSql="insert into [%1] (cmd_id,tag,cmd_str,create_time) values (%2,%3,'%4',datetime('now'))";
        return execSql(strSql.arg( nQueue==QUEUE_IN ? TABLE_SQIN : TABLE_SQOUT ).arg(strCmdID).arg(TAG_UNSEND).arg(strCmdStr));
}

int SyncDB::cmd_tag(QString strCmdID,int tag,QString strCmdRet,QUEUE_ID nQueue)
{
	QString strTime;
	switch(tag)
	{
	case TAG_UNSEND:
		strTime="";
		break;
	case TAG_SENDING:
		strTime=",send_time=datetime('now')";
		break;
	default:
		strTime=",end_time=datetime('now')";
	}
	QString strSql="update [%1] set tag=%2%5,cmd_ret='%3' where cmd_id=%4";
        return execSql(strSql.arg( nQueue==QUEUE_IN ? TABLE_SQIN : TABLE_SQOUT).arg(tag).arg(strCmdRet).arg(strCmdID).arg(strTime));
}

bool SyncDB::cmd_exist(QString strCmdID,QUEUE_ID nQueue)
{
	QString strSql="select count(*) as cnt from %1 where cmd_id=%2";
        CppSQLite3Query result=querySql(strSql.arg( nQueue==QUEUE_IN ? TABLE_SQIN : TABLE_SQOUT).arg(strCmdID));
	if(!result.eof())
	{
		if(!result.fieldIsNull(0))
		{
			return (QString(result.getStringField(0)).toInt() > 0);
		}
	}
	return false;
}

CommandMap SyncDB::cmd_get(QString strCmdID,QUEUE_ID nQueue)
{
	CommandMap props;
	QString strSql;
	strSql="select cmd_str from %1 where cmd_id=%2 limit 1";
        CppSQLite3Query result=querySql(strSql.arg( nQueue==QUEUE_IN ? TABLE_SQIN : TABLE_SQOUT).arg(strCmdID));
	if(!result.eof())
	{
		if(!result.fieldIsNull(0))
		{
			props=convert_from_cmdline(result.getStringField(0));
		}
	}
	return props;
}


CommandMap SyncDB::cmd_get_ret(QString strCmdID,QUEUE_ID nQueue)
{
	CommandMap props;
	QString strSql;
	strSql="select cmd_ret from %1 where cmd_id=%2 limit 1";
        CppSQLite3Query result=querySql(strSql.arg( nQueue==QUEUE_IN ? TABLE_SQIN : TABLE_SQOUT).arg(strCmdID));
	if(!result.eof())
	{
		if(!result.fieldIsNull(0))
		{
			props=convert_from_cmdline(result.getStringField(0));
		}
	}
	return props;
}
int SyncDB::cmd_reset_queue()
{
	QString strSql;
	strSql="update [%1] set tag=%2 where tag=%3 and cmd_str like '%4%'";
        execSql(strSql.arg(TABLE_SQOUT).arg(TAG_ABANDON).arg(TAG_SENDING).arg(get_cmdstr(CMD_HELLO)));
	strSql="update [%1] set tag=%2 where tag=%3";
        int ret = execSql(strSql.arg(TABLE_SQOUT).arg(TAG_UNSEND).arg(TAG_SENDING)) +
        execSql(strSql.arg(TABLE_SQIN).arg(TAG_UNSEND).arg(TAG_SENDING));
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

bool SyncDB::tableExists(QString strTable)
{
	return m_db.tableExists(strTable.toStdString().c_str());
}

