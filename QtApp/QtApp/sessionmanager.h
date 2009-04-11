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
	//公开接口和槽，全部要使用锁同步线程
public:
	QString generate_cmdid();
signals:
	void sigLogger(QString);

public slots:
	void heartbeat();
	void ConnectHost();
	void DisconnectHost();
	void client_connected(Client *cl);
	void client_disconnected(Client *cl);
private:
	bool bAutoConnectHost;
	QString svrhost;
	QString svrport;
	SyncDB *syncdb;
	Synconf *synconf;
	QTimer *timer;
	quint32 heartbeat_count;
	quint32 heartbeat_interval;
	QMutex m_locker; //普通锁，同步对自身资源的操作
public:
	Client client;
};

#endif // SESSIONMANAGER_H
