#include "sessionmanager.h"

SessionManager::SessionManager(QObject *parent)
	: QObject(parent)
	,heartbeat_count(0)
	,bAutoConnectHost(false)//���Զ�����
	,client(this)
	,ping_cmdid(0)
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
		connect(&client,SIGNAL(sigRecv(Client *,quint32,QString ,QString ,CommandMap ,QByteArray )),
			this,SLOT(recv_data(Client *,quint32,QString ,QString ,CommandMap ,QByteArray )));

		syncdb->reset_cmd_queue();

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
	syncdb->reset_cmd_queue();
}

//��������ID
QString SessionManager::generate_cmdid()
{
	QMutexLocker locker(&m_locker);
	quint32 cmdid=synconf->getstr("next_cmdid","1").toInt();
	
	CommandMap mp=syncdb->singleQuerySql("select max(cmd_id) as mid from sqout");
	quint32 maxid=mp["mid"].toInt();
	if(maxid>cmdid)
		cmdid=maxid+1;
	synconf->setstr("current_cmdid",QString::number(cmdid+1));
	return QString::number(cmdid);
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
		say_ping(generate_cmdid());
	}
	//TODO:�����ﴦ��δ���͵�����

}

void SessionManager::client_connected(Client *cl)
{
	say_hello(Client *cl);
}

void SessionManager::client_disconnected(Client *cl)
{
}

//���� nCmdID 
int SessionManager::send_data(CommandMap mapCmdLine,QByteArray buffer)
{
//TODO:����Ҫһ��send_data���������ز�����ҵ�������Client�� say��䶼Ǩ�Ƶ�SessionManager��ʵ��	
	return 0;
}

void SessionManager::recv_data(Client *cl,quint32 nCmdID,QString strCmdStr,QString strCmdLine,CommandMap props ,QByteArray buffer)
{
	QString str=QString::fromLocal8Bit("�յ����%1,%2,%3,%4,%5");
	QByteArray st=str.arg(nCmdID).arg(strCmdStr).arg(strCmdLine).arg(props.size()).arg(buffer.size()).toLocal8Bit();
	qDebug(st.data());
	emit sigLogger(st);

	//������Ӧstate���������Ӧ��������У���������ʱ���ڸ��ӵ����񣬿��ǿ��߳�ȥ����
	if(nCmdID != CMD_STATE && nCmdID != CMD_UNKNOWN )
	{
		if(syncdb->exist_cmd(props["1"]),QUEUE_IN)
		{
			//�յ�������֮ǰ�Ѿ��յ�����������Ѿ���ɵ��������ݵģ����ٴη��ͽ��
			//�����������ظ�����
			//TODO
			return ;
		}
		syncdb->put_cmd(props["1"],strCmdLine,QUEUE_IN);
	}

	switch(nCmdID)
	{
	case CMD_BYE:
		{
			if(cl->ack_bye(props)==ERR_NONE)
			{
				syncdb->tag_cmd(props["1"],STA_COMPLETE,QUEUE_IN);
			}
			else
			{
				syncdb->tag_cmd(props["1"],STA_ERROR,QUEUE_IN);
			}
		}
	case CMD_STATE:
		{
			//�յ�������Ӧ�Ĵ�����
			//�ӷ��Ͷ������Ҹ��������Ҳ��������������Ѿ���ɣ����������Ǽ�¼��־�������޸�����Ϊ���
			//�����PING�������Ӧ������������У�����Ӧ���ڳ�Ա�������¼�ͱȽ�
			//TODO
		}
		break;
	case CMD_ALERT:
		{
			//����ע�������¼�֪ͨ������Ҫ���ϲ�ð��
			//TODO
		}
		break;
	case CMD_WHOAREYOU:
		{
			CommandMap ack_props;
			ack_props["1"]=props["1"];//��Ӧ������ID
			ack_props["sessionid"]=synconf->getstr("session_id");
			ack_props["user"]=synconf->getstr("user_id");
			ack_props["pwd"]=synconf->getstr("user_password");
			ack_props["devuid"]=synconf->getstr("device_id");
			if(cl->ack_state(ack_props)==ERR_NONE)
			{
				syncdb->tag_cmd(ack_props["1"],STA_COMPLETE,QUEUE_IN);
			}
			else
			{
				syncdb->tag_cmd(ack_props["1"],STA_ERROR,QUEUE_IN);
			}
		}
		break;
	case CMD_UNKNOWN:
	default:
		emit sigLogger("����δ֪���"+str);
		break;
	}

}

// �ⷢϵ��Э������ʵ��
int SessionManager::say_hello(Client *cl)
{
	CommandMap props;
	//hello 101 syncany_client=2.1.0.0 protocol=1.0.1.0 platform=symbian-os-s60.3
	props["syncany_client"]=synconf->getinfo("client_version");
	props["protocol"]=synconf->getinfo("protocol_version");
	props["platform"]=synconf->getinfo("os_version");
	porps["0"]=get_cmdstr(CMD_HELLO);
	props["1"]=generate_cmdid();

	QString strCmdLine=convert_to_cmdline(props);
	syncdb->put_cmd(props["1"],strCmdLine,QUEUE_OUT);
	int ret=cl->sendData(strCmdLine);
	if(ret==ERR_NONE)
		syncdb->tag_cmd(props["1"],STA_SENDING);
	//else //send fail....?
	//  
	return ret;
}

int SessionManager::say_ping(Client *cl)
{
	QString strCmdLine("ping ");
	strCmdLine+=generate_cmdid();
	strCmdLine+="\n";
	return cl->sendData(strCmdLine);
}


int SessionManager::ack_state(Client *cl,CommandMap props)
{
	return ack_state(cl,props,QByteArray());
}

int SessionManager::ack_state(Client *cl,CommandMap props,QByteArray data)
{
	if(data.length()>0 )
	{
		//δָ�����ȣ�����֮
		props["size"]=data.length();
	}
	else
	{
		if(props.find("size")!=props.end())
			props.remove("size");
	}
	props["0"]=get_cmdstr(CMD_STATE);

	QString cmdbuf=convert_to_cmdline(props);
	if(data.length()>0)
		cmdbuf.append(data);
	return sendData(cmdbuf);
}


int SessionManager::ack_bye(Client *cl,CommandMap props)
{
	int ret=ERR_UNKNOWN;
	if(props.find("1")!=props.end())
	{
		CommandMap ack_props;
		ack_props["1"]=props["1"];


		QString strCmdID=props["1"];
		if(strCmdID!=0 && strCmdID.toInt()!=0)
		{
			QString strCmdStr="state "+strCmdID+" 200\n";
			ret=sendData(strCmdStr);
			m_sock->waitForBytesWritten(100);
		}
	}
	DisconnectHost();
	return ret;
}

