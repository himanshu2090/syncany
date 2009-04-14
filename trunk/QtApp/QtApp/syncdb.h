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
	int put_cmd(QString strCmdID,QString strCmdLine,QUEUE_ID nQueue=QUEUE_IN);
	int tag_cmd(QString strCmdID,int tag,QUEUE_ID nQueue=QUEUE_IN);
	int tag_cmd(QString strCmdID,int tag,QString strCmdRet,QUEUE_ID nQueue=QUEUE_IN);
	bool exist_cmd(QString strCmdID,QUEUE_ID nQueue=QUEUE_IN);
	int reset_cmd_queue();//������δ��ɵ�����תΪδ����״̬���ڳ����˳�������ʱִ�У��Ա��ط�����
	CommandMap get_cmd(int tag,QUEUE_ID nQueue=QUEUE_IN);
	CommandMap get_cmd(QString strCmdID,QUEUE_ID nQueue=QUEUE_IN);

	//��ͬ����SQL���ִ�нӿ�
	int execSql(QString strSql);
	int execSql(QByteArray strSql);
	CppSQLite3Query querySql(QByteArray strSql);
	CppSQLite3Query querySql(QString strSql);
	CommandMap singleQuerySql(QString strSql);

private:
	int m_type;
	QString m_strTable;
	QString m_dbFile;
	CppSQLite3DB m_db;
};

#endif // SYNCQUEUE_H
