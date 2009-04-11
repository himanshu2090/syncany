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

		svrhost=synconf->getstr("server_host","5.40.92.214");
		svrport=synconf->getstr("server_port","18120");
		synconf->setstr("server_host",svrhost);
		synconf->setstr("server_port",svrport);

		QString strTemp=synconf->getstr("heartbeat_interval","900");
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
	++cmdid;
	QString strCmdID(cmdid);
	synconf->setstr("current_cmdid",strCmdID);
	return strCmdID;
}

void SessionManager::ConnectHost()
{
	if(client.getSocket()->state()!=QTcpSocket::ConnectedState)
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

