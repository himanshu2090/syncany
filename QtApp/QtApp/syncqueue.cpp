#include "syncqueue.h"

SyncQueue::SyncQueue(QObject *parent,int type)
	: QObject(parent),m_type(type),m_strTable((type==0)?"sqin":"sqout")
{
	createTable();
}

SyncQueue::~SyncQueue()
{

}

SyncQueue *SyncQueue::instance(int type)
{
	if(type==0)
	{
		QMutexLocker locker(m_locker_queue_in);
		if(!m_instance_in)
		{
			m_instance_in=new SyncQueue(type);
			m_instance_in->m_locker=&m_locker_queue_in;
		}
		return m_instance_in;
	}
	else if(type==1)
	{
		QMutexLocker locker(m_locker_queue_out);
		if(!m_instance_out)
		{
			m_instance_out=new SyncQueue(type);
			m_instance_out->m_locker=&m_locker_queue_out;
		}
		return m_instance_out;
	}
	else
		return 0;
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

void SyncQueue::createTable() //�ڹ��캯������ã�δ��������Ϊ�ڴ�������ʵ��ǰ�Ѿ�����
{
	if(m_db.tableExists(m_strTable.toStdString().c_str()))
		return ;
	QString strSql="CREATE TABLE [%1] (\n"
		"[id] AUTOINC, \n"
		"[cmd_id] INT NOT NULL, \n"
		"[tag] INT NOT NULL DEFAULT 0, \n"
		"[cmd_str] VARCHAR NOT NULL, \n"
		"[cmd_ret] VARCHAR, \n"
		"[create_time] DATETIME,\n"
		"[send_time] DATETIME, \n"
		"[end_time] DATETIME, \n"
		"CONSTRAINT [sqlite_autoindex_%1_1] PRIMARY KEY ([cmd_id]));\n";
	strSql.arg(m_strTable);
	m_db.execDML(strSql.toStdString().c_str());
}


void SyncQueue::put_cmd(QString strCmdID,QString strCmdStr)
{
	QString strSql="insert into [%1] (cmd_id,tag,cmd_str,create_time) value(%2,0,%3,now())";
}

void SyncQueue::tag_cmd(QString strCmdID,int tag)
{
}

void SyncQueue::tag_cmd(QString strCmdID,int tag,QString strCmdRet)
{
}

