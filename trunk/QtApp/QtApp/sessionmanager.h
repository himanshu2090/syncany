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
signals:
	void sigLogger(QString);

public slots:
	void heartbeat();
	void ConnectHost();
	void DisconnectHost();
	void client_connected(Client *cl);
	void client_disconnected(Client *cl);
	int send_data(CommandMap mapCmdLine,QByteArray buffer);
	void recv_data(Client *,quint32 nCmdID,QString strCmdStr,QString strCmdLine,CommandMap props,QByteArray buffer);

public:
	int say_ping(Client *cl);
	int say_hello(Client *cl);
	int ack_bye(Client *cl,CommandMap props);
	int ack_state(Client *cl,CommandMap props,QByteArray data=QByteArray());//�ɴ������ݵİ汾

	int do_job(CommandMap props,Client *cl);
	int do_sendcmd(CommandMap props,Client *cl);
private:
	void dispatch_task(); //��δ��ɵ�����ַ����ͻ��˵�����ģ�������
	void resend_cmd();//��δ���͵��������·���

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
	QMutex m_locker_cmdid; //ͬ����������cmdid�Ĳ���
	int ping_cmdid;
	QDateTime ping_acktime;
	int timeout_secs;
	Client *ping_cl;
public:
	Client client;
};

#endif // SESSIONMANAGER_H


