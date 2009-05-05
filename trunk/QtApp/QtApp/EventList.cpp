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

void EventList::createTable()
{
	SyncDB *syncdb=SyncDB::instance();
	if(!syncdb->tableExists(strTableName))
	{
		QString strSql;
		strSql+="CREATE TABLE [";
		strSql+=strTableName+"] (\n"
			"[eid] AUTOINC, \n"
			"[url] VARCHAR NOT NULL, \n"
			"[syncdir] INT NOT NULL, \n"
			"[filename] VARCHAR ,\n" 
			"[filesize] INT , \n"
			"[anchor] INT, \n"
			"[anchor_time] DATETIME, \n"
			"[modify_time] DATETIME,\n"
			"[event_time] DATETIME,\n"
			"CONSTRAINT [sqlite_autoindex_"+strTableName+"_url_syncdir] PRIMARY KEY ([url,syncdir]));\n";
		syncdb->execSql(strSql);
	}
}

quint32 EventList::insertEvent(PtrFile ptrfile,int syncdir)
{
	QMutexLocker locker(&g_locker_eventlist);
	QString strSql;
	int ret=ERR_NONE;
	strSql="select * from "+strTableName+" where url='"+ptrfile->getUrl()+"'";
	CommandMap props=syncdb->singleQuerySql(strSql);
	if(props.size()==0)
	{
		//新增
		strSql="insert into "+strTableName+" (url,syncdir,filename,filesize,anchor,anchor_time,modify_time,event_time) "
			"values ('"+ptrfile->getUrl()+"',"+QString::number(syncdir)+",'"+ptrfile->getLocalUrl()+"',"+QString::number(ptrfile->getSize())+","+QString::number(ptrfile->getAnchor())+",'"+ptrfile->getAnchorTime().toString()+"','"+ptrfile->getLastModifyTime().toString()+"',datetime('now'))";
		return syncdb->execSql(strSql);
	}
	if(props["syncdir"].toInt()==syncdir)//方向不同的事件同时发生，意味着发生冲突
	{
		//事件重复
		strSql="update "+strTableName+" set event_time=datetime('now') where url='"+ptrfile->getUrl()+"' and syncdir="+QString::number(syncdir);
		return syncdb->execSql(strSql);
	}
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

