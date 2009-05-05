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

//�����¼����б�
/*
�����ֶΣ�
url		URL��־��Ψһ
syncdir ͬ������0��ͬ���¼����Ѿ�ͬ����ɣ�1�������ͬ����2�򱾵�ͬ����
cmdid   ����ID���ɿգ�
anchor  ê��Ϣ
anchor_time  �ڱ�����дê���¼ʱ���ļ�����޸�ʱ��
modify_time  ��¼�����¼�����ʱ���ļ�����޸�ʱ��
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
		//����
		strSql="insert into "+strTableName+" (url,syncdir,filename,filesize,anchor,anchor_time,modify_time,event_time) "
			"values ('"+ptrfile->getUrl()+"',"+QString::number(syncdir)+",'"+ptrfile->getLocalUrl()+"',"+QString::number(ptrfile->getSize())+","+QString::number(ptrfile->getAnchor())+",'"+ptrfile->getAnchorTime().toString()+"','"+ptrfile->getLastModifyTime().toString()+"',datetime('now'))";
		return syncdb->execSql(strSql);
	}
	if(props["syncdir"].toInt()==syncdir)//����ͬ���¼�ͬʱ��������ζ�ŷ�����ͻ
	{
		//�¼��ظ�
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

