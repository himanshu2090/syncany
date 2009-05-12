#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include "common.h"

#include "client.h"
#include "syncdb.h"
#include "synconf.h"
#include "itraymessage.h"

class SessionManager : public ITrayMessage
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
	int send_data(CommandMap mapCmdLine,QByteArray buffer);
	void recv_data(Client *,quint32 nCmdID,QString strCmdStr,QString strCmdLine,CommandMap props,QByteArray buffer);

public:
	int say_ping(Client *cl);
	int say_hello(Client *cl);
	int ack_bye(Client *cl,CommandMap props);
	int ack_state(Client *cl,CommandMap props,QByteArray data=QByteArray());//可传输数据的版本

	int do_job(Client *cl,CommandMap props,QByteArray data=QByteArray());
	int do_state(Client *cl,CommandMap state_props,CommandMap cmd_props,QByteArray data=QByteArray());
	int do_sendcmd(Client *cl,CommandMap props,QByteArray data=QByteArray());

	CommandMap syncSend(Client *cl,QString strCmdID,QString strCmdLine,QByteArray buffer=QByteArray());
public:
	QList<QString> cmd_waiter;
	QList<PtrFile> ls_file(QString strUrl);
	quint32 put_file(PtrFile pf);
	quint32 rm_file(PtrFile pf);
private:
	void dispatch_task(); //将未完成的任务分法给客户端的其他模块来完成
	void resend_cmd();//将未发送的命令重新发送
	void update_ping_acktime();
private:
	bool bAutoConnectHost;
	QString svrhost;
	QString svrport;
	SyncDB *syncdb;
	Synconf *synconf;
	QPointer<QTimer> timer;
	quint32 heartbeat_count;
	quint32 heartbeat_interval;
	QMutex m_locker; //普通锁，同步对自身资源的操作
	QMutex m_locker_cmdid; //同步锁，控制cmdid的产生
	int ping_cmdid;
	QDateTime ping_acktime;
        quint32 timeout_secs;
	Client *ping_cl;
	QString strTempDirectory;

public:
	Client client;
};

#endif // SESSIONMANAGER_H


