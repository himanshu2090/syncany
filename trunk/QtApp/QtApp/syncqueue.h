#ifndef SYNCQUEUE_H
#define SYNCQUEUE_H

#include <QtCore/QtCore>
#include "CppSQLite3.h"

//接收和发送队列，构造为两个单体类来提供数据持久化支持
QMutex m_locker_queue_in;
QMutex m_locker_queue_out;

class SyncQueue : public QObject
{
	Q_OBJECT
private:
	SyncQueue(){};
	SyncQueue(QObject *parent=0,type);
public:
	~SyncQueue();
	
private:
	void createTable();
public:
	static SyncQueue *m_instance_in;
	static SyncQueue *m_instance_out;
	static SyncQueue *instance(int type=0);//返回不同的命令单体队列
public slots:
	void put_cmd(QString strCmdID,QString strCmdStr);
	void tag_cmd(QString strCmdID,int tag);
	void tag_cmd(QString strCmdID,int tag,QString strCmdRet);
	
private:
	int m_type;
	QMutex *m_locker;
	QString m_strTable;
	CppSQLite3DB m_db;
};

#endif // SYNCQUEUE_H
