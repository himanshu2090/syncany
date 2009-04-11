#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include "common.h"

#include "client.h"
#include "syncdb.h"
#include "synconf.h"

class SessionManager : public QObject
{
	Q_OBJECT

public:
	SessionManager(QObject *parent);
	~SessionManager();
	//�����ӿںͲۣ�ȫ��Ҫʹ����ͬ���߳�
public:
	QString generate_cmdid();
	void sigLogger(QString);

public slots:
	void heartbeat();
	void ConnectHost();
	void DisconnectHost();
private:
	bool bAutoConnectHost;
	QString svrhost;
	QString svrport;
	SyncDB *syncdb;
	Synconf *synconf;
	QTimer *timer;
	quint32 heartbeat_count;
	quint32 heartbeat_interval;
	QMutex m_locker; //��ͨ����ͬ����������Դ�Ĳ���
	Client client;
};

#endif // SESSIONMANAGER_H
