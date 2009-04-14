#include "sessionmanager.h"

SessionManager::SessionManager(QObject *parent)
	: QObject(parent)
	,heartbeat_count(0)
	,bAutoConnectHost(false)//不自动连接
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

		//日志网络连接的信息
		connect(&client,SIGNAL(sigLogger(QString)),parent,SLOT(log(QString)));
		connect(this,SIGNAL(sigLogger(QString)),parent,SLOT(log(QString)));

		connect(&client,SIGNAL(sigIn(QString)),parent,SLOT(login(QString)));
		connect(&client,SIGNAL(sigOut(QString)),parent,SLOT(logout(QString)));

		//跟踪管理的client的连接和断开信号
		connect(&client,SIGNAL(connected(Client *)),this,SLOT(client_connected(Client *)));
		connect(&client,SIGNAL(disconnected(Client *)),this,SLOT(client_disconnected(Client *)));

		//将client传入的内容根据协议解析出命令或响应后再由SM进行调度
		connect(&client,SIGNAL(sigRecv(Client *,quint32,QString ,QString ,CommandMap ,QByteArray )),
			this,SLOT(recv_data(Client *,quint32,QString ,QString ,CommandMap ,QByteArray )));

		syncdb->reset_cmd_queue();

		//传出的命令或响应由SM调度交给各个client发送

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

//管理命令ID
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
		client.say_ping(generate_cmdid());
	}
	//TODO:在这里处理未发送的命令

}

void SessionManager::client_connected(Client *cl)
{
	CommandMap props;
	//hello 101 syncany_client=2.1.0.0 protocol=1.0.1.0 platform=symbian-os-s60.3
	props["syncany_client"]=synconf->getinfo("client_version");
	props["protocol"]=synconf->getinfo("protocol_version");
	props["platform"]=synconf->getinfo("os_version");

	cl->say_hello(generate_cmdid(),props);
}

void SessionManager::client_disconnected(Client *cl)
{
}

//返回 nCmdID 
int SessionManager::send_data(CommandMap mapCmdLine)
{
//TODO:还需要一个send_data，来处理本地产生的业务命令，将Client的 say语句都迁移到SessionManager里实现	
	return 0;
}

void SessionManager::recv_data(Client *cl,quint32 nCmdID,QString strCmdStr,QString strCmdLine,CommandMap props ,QByteArray buffer)
{
	QString str=QString::fromLocal8Bit("收到命令：%1,%2,%3,%4,%5");
	QByteArray st=str.arg(nCmdID).arg(strCmdStr).arg(strCmdLine).arg(props.size()).arg(buffer.size()).toLocal8Bit();
	qDebug(st.data());
	emit sigLogger(st);

	//除了响应state，其余命令都应该先入队列，后续处理时对于复杂的任务，考虑开线程去处理
	if(nCmdID != CMD_STATE && nCmdID != CMD_UNKNOWN )
	{
		if(syncdb->exist_cmd(props["1"]),QUEUE_IN)
		{
			//收到的命令之前已经收到过，如果是已经完成的且无数据的，则再次发送结果
			//否则反馈命令重复错误
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
			//收到命令相应的处理结果
			//从发送队列里找该命令，如果找不到，或者命令已经完成，则丢弃，但是记录日志，否则修改命令为完成
			//如果是PING命令的响应，则不入命令队列，所以应该在成员变量里记录和比较
			//TODO
		}
		break;
	case CMD_ALERT:
		{
			//处理注册结果，事件通知——需要向上层冒泡
			//TODO
		}
		break;
	case CMD_WHOAREYOU:
		{
			CommandMap props;
			props["sessionid"]=synconf->getstr("session_id");
			props["user"]=synconf->getstr("user_id");
			props["pwd"]=synconf->getstr("user_password");
			props["devuid"]=synconf->getstr("device_id");
			if(cl->ack_state(props)==ERR_NONE)
			{
				syncdb->tag_cmd(props["1"],STA_COMPLETE,QUEUE_IN);
			}
			else
			{
				syncdb->tag_cmd(props["1"],STA_ERROR,QUEUE_IN);
			}
		}
		break;
	case CMD_UNKNOWN:
	default:
		emit sigLogger("错误！未知命令："+str);
		break;
	}

}
