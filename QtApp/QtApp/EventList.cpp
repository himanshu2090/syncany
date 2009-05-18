#include "EventList.h"

EventList * EventList::m_instance=null;
QMutex g_locker_eventlist;

EventList::EventList(void)
{
	syncdb=SyncDB::instance();
	synconf=Synconf::instance();
}

EventList::~EventList(void)
{
}

EventList *EventList::instance()
{
	QMutexLocker locker(&g_locker_eventlist);
	if(m_instance==null)
	{
		m_instance=new EventList();
		m_instance->createTable();
	}
	return m_instance;
}

//创建事件队列表
/*
包含字段：
url		URL标志，唯一
syncdir 同步方向（0非同步事件或已经同步完成，1向服务器同步，2向本地同步）
cmdid   命令ID（可空）
anchor  锚信息
anchor_time  在本地填写锚点记录时的文件最后修改时间
modify_time  记录本次事件发生时的文件最后修改时间
*/

QString EventList::strTableName="event_list";

/*
CREATE TABLE [event_list] (
	[eid] AUTOINC default 1, 
	[url] VARCHAR NOT NULL, 
	[syncdir] INT NOT NULL, 
	[syncop] INT NOT NULL,
	[filename] VARCHAR ,
	[filesize] INT , 
	[anchor] INT, 
	[anchor_time] DATETIME, 
	[modify_time] DATETIME,
	[event_time] DATETIME,
	CONSTRAINT [sqlite_autoindex_event_list_url_syncdir] PRIMARY KEY ([url],[syncdir]));
*/

void EventList::createTable()
{
	SyncDB *syncdb=SyncDB::instance();
	if(!syncdb->tableExists(strTableName))
	{
		QString strSql;
		strSql+="CREATE TABLE [";
		strSql+=strTableName+"] (\n"
			"[eid] INTEGER PRIMARY KEY AUTOINCREMENT , \n"
			"[url] VARCHAR NOT NULL, \n"
			"[syncdir] INT NOT NULL, \n"
			"[syncop] INT NOT NULL,\n"
			"[filename] VARCHAR ,\n" 
			"[filesize] INT , \n"
			"[anchor] INT, \n"
			"[anchor_time] DATETIME, \n"
			"[modify_time] DATETIME,\n"
			"[event_time] DATETIME);\n";
		syncdb->execSql(strSql);
		strSql="CREATE UNIQUE INDEX [event_list_url_syncdir] ON [event_list] ([url], [syncdir]);";
		syncdb->execSql(strSql);
	}
}

quint32 EventList::insertEvent(PtrFile ptrfile,int syncdir,int syncop)
{
	QMutexLocker locker(&g_locker_eventlist);
	QString strSql;
	int ret=ERR_NONE;
	strSql="select * from "+strTableName+" where url='"+ptrfile->getUrl()+"'";
	CommandMap props=syncdb->singleQuerySql(strSql);
	if(props.size()==0)
	{
		//新增
		strSql="insert into "+strTableName+" (url,syncdir,syncop,filename,filesize,anchor,anchor_time,modify_time,event_time) "
			"values ('"+ptrfile->getUrl()+"',"+QString::number(syncdir)+","+QString::number(syncop)+",'"+ptrfile->getLocalUrl()+"',"+QString::number(ptrfile->getSize())+","+QString::number(ptrfile->getAnchor())+",'"+ptrfile->getAnchorTime().toString()+"','"+ptrfile->getLastModifyTime().toString()+"',datetime('now'))";
		return syncdb->execSql(strSql);
	}
	if(props["syncdir"].toInt()==syncdir)//方向不同的事件同时发生，意味着发生冲突
	{
		//事件重复
		strSql="update "+strTableName+" set event_time=datetime('now'),syncop="+QString::number(syncdir)+" where url='"+ptrfile->getUrl()+"' and syncdir="+QString::number(syncdir);
		return syncdb->execSql(strSql);
	}
	else
		emit trayMessage(QObject::tr("conflict!"),ptrfile->getUrl()+QObject::tr("conflict with server's copy!"));
	return ret;
}

quint32 EventList::removeEvent(QString strUrl,int syncdir)
{
	QMutexLocker locker(&g_locker_eventlist);
	QString strSql;
	strSql="delete from "+strTableName+" where url='"+strUrl+"' and syncdir="+QString::number(syncdir);
	syncdb->execSql(strSql);
	return ERR_NONE;
}

quint32 EventList::getNextEvent(int &eid,QString &strUrl,int &syncdir,int &syncop,bool remove)
{
	QMutexLocker locker(&g_locker_eventlist);
	QString strSql="select * from "+strTableName+" where eid>"+QString::number(eid);
	strSql+=" limit 1";//每次只取一行
	CommandMap props=syncdb->singleQuerySql(strSql);
	if(props.size()==0)
		return ERR_NODATA;
	eid=props["eid"].toInt();
	strUrl=props["url"];
	syncdir=props["syncdir"].toInt();
	syncop=props["syncop"].toInt();

	if(remove)
	{
		removeEvent(strUrl,syncdir);
	}
	return ERR_NONE;
}

