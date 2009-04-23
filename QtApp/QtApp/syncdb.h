#ifndef SYNCQUEUE_H
#define SYNCQUEUE_H

#include "common.h"

//���պͷ��Ͷ��У�����Ϊһ�����������ṩ���ݳ־û�֧��
//�������ID
enum QUEUE_ID
{
	QUEUE_IN=0,
	QUEUE_OUT=1,
};

static const char* strQueueTableName[]=
{
	"sqin",
	"sqout"
};

enum SYNC_TYPES
{
	SYNC_FILES=0,
};

static const char* strSyncTableName[]=
{
	"sync_files",
};

class SyncDB : public QObject
{
	Q_OBJECT
private:
	SyncDB(QObject *parent=0);
public:
	~SyncDB();
	
private:
	void createTable();
public:
	static SyncDB *m_instance;
	static SyncDB *instance();//���ز�ͬ����������

public slots:
	//������صĲ۽ӿ�
	int cmd_put(QString strCmdID,QString strCmdLine,QUEUE_ID nQueue=QUEUE_IN);
	int cmd_tag(QString strCmdID,int tag,QString strCmdRet=QString(""),QUEUE_ID nQueue=QUEUE_IN);
	bool cmd_exist(QString strCmdID,QUEUE_ID nQueue=QUEUE_IN);
	int cmd_reset_queue();//������δ��ɵ�����תΪδ����״̬���ڳ����˳�������ʱִ�У��Ա��ط�����
	CommandMap cmd_get(QString strCmdID,QUEUE_ID nQueue=QUEUE_IN);

	//��ͬ����SQL���ִ�нӿ�
	int execSql(QString strSql);
	int execSql(QByteArray strSql);
	CppSQLite3Query querySql(QByteArray strSql);
	CppSQLite3Query querySql(QString strSql);
	CommandMap singleQuerySql(QString strSql);
	bool tableExists(QString);
private:
	int m_type;
	QString m_strTable;
	QString m_dbFile;
	CppSQLite3DB m_db;
};

#endif // SYNCQUEUE_H
