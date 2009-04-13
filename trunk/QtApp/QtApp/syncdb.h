#ifndef SYNCQUEUE_H
#define SYNCQUEUE_H

#include "common.h"

//���պͷ��Ͷ��У�����Ϊһ�����������ṩ���ݳ־û�֧��


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
	void put_cmd(QString strCmdID,QString strCmdStr);
	void tag_cmd(QString strCmdID,int tag);
	void tag_cmd(QString strCmdID,int tag,QString strCmdRet);
	void execSql(QString strSql);

private:
	int m_type;
	QString m_strTable;
	QString m_dbFile;
	CppSQLite3DB m_db;
};

#endif // SYNCQUEUE_H
