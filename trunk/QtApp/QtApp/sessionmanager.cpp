#include "sessionmanager.h"

SessionManager::SessionManager(QObject *parent)
	: QObject(parent)
	,heartbeat_count(0)
	,bAutoConnectHost(false)//���Զ�����
	,client(this)
{
	try
	{
		timer = null;
		syncdb=SyncDB::instance();
		synconf=Synconf::instance();

		svrhost=synconf->getstr("server_host","5.7.217.164");
		svrport=synconf->getstr("server_port","18120");
		synconf->setstr("server_host",svrhost);
		synconf->setstr("server_port",svrport);

		QString strTemp=synconf->getstr("heartbeat_interval","10");
		synconf->setstr("heartbeat_interval",strTemp,true);
		heartbeat_interval=strTemp.toInt();

		//��־�������ӵ���Ϣ
		connect(&client,SIGNAL(sigLogger(QString)),parent,SLOT(log(QString)));
		connect(this,SIGNAL(sigLogger(QString)),parent,SLOT(log(QString)));

		connect(&client,SIGNAL(sigIn(QString)),parent,SLOT(login(QString)));
		connect(&client,SIGNAL(sigOut(QString)),parent,SLOT(logout(QString)));

		//���ٹ����client�����ӺͶϿ��ź�
		connect(&client,SIGNAL(connected(Client *)),this,SLOT(client_connected(Client *)));
		connect(&client,SIGNAL(disconnected(Client *)),this,SLOT(client_disconnected(Client *)));

		//��client��������ݸ���Э��������������Ӧ������SM���е���
		connect(&client,SIGNAL(sigRecv(Client *,QString ,QString ,QMap<QString,QString> ,QByteArray )),
			this,SLOT(recv_data(Client *,QString ,QString ,QMap<QString,QString> ,QByteArray )));

		//�������������Ӧ��SM���Ƚ�������client����

		timer = new QTimer(this);
		connect(timer, SIGNAL(timeout()), this, SLOT(heartbeat()));
		timer->start(1000);

		
	}
	catch(...)
	{
		if(timer!=null)
			delete timer;
		emit sigLogger("Session Manager Constructor fail!");
	}
}

SessionManager::~SessionManager()
{
	if(timer!=null)
		delete timer;
}

//��������ID
QString SessionManager::generate_cmdid()
{
	QMutexLocker locker(&m_locker);
	quint32 cmdid=synconf->getstr("current_cmdid","1").toInt();
	QString strRet;
	strRet.setNum(cmdid);
	++cmdid;
	QString strCmdID;
	strCmdID.setNum(cmdid);
	synconf->setstr("current_cmdid",strCmdID);
	return strRet;
}

void SessionManager::ConnectHost()
{
	bAutoConnectHost=true;
	QTcpSocket::SocketState sta=client.getSocket()->state();
	if(sta!=QTcpSocket::ConnectedState && sta != QTcpSocket::ConnectingState)
	{
		svrhost=synconf->getstr("server_host","");
		svrport=synconf->getstr("server_port","");
		if(svrhost!="" && svrport!="")
		{
			client.ConnectHost(svrhost,svrport.toInt());
		}
	}
}

void SessionManager::DisconnectHost()
{
	bAutoConnectHost=false;
	client.DisconnectHost();
}

void SessionManager::heartbeat()
{
	if(bAutoConnectHost)
	{
		ConnectHost();
	}
	if(++heartbeat_count > heartbeat_interval)
	{
		heartbeat_count=0;
		client.say_ping(generate_cmdid());
	}
	//TODO:�����ﴦ��δ���͵�����
}

void SessionManager::client_connected(Client *cl)
{
	QMap<QString,QString> props;
	//hello 101 syncany_client=2.1.0.0 protocol=1.0.1.0 platform=symbian-os-s60.3
	props["syncany_client"]=synconf->getinfo("client_version");
	props["protocol"]=synconf->getinfo("protocol_version");
	props["platform"]=synconf->getinfo("os_version");

	cl->say_hello(generate_cmdid(),props);
}

void SessionManager::client_disconnected(Client *cl)
{
}

void SessionManager::recv_data(Client *cl,quint32 nCmdID,QString strCmdStr,QString strCmdLine,QMap<QString,QString> props ,QByteArray buffer)
{
	QString str=QString::fromLocal8Bit("�յ����%1,%2,%3,%4,%5");
	QByteArray st=str.arg(nCmdID).arg(strCmdStr).arg(strCmdLine).arg(props.size()).arg(buffer.size()).toLocal8Bit();

	switch(nCmdID)
	{
	case CMD_BYE:
		cl->say_bye(props);
	case CMD_STATE:
		//�յ�������Ӧ�Ĵ���
		break;
	case CMD_ALERT:
		break;
	case CMD_WHOAREYOU:
		break;
	case CMD_UNKNOWN:
	default:
		emit sigLogger("����δ֪���"+str);
		break;
	}

	qDebug(st.data());
	emit sigLogger(str);
}
