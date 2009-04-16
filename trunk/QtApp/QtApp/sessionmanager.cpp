#include "sessionmanager.h"

SessionManager::SessionManager(QObject *parent)
	: QObject(parent)
	,heartbeat_count(0)
	,bAutoConnectHost(false)//不自动连接
	,client(this)
	,ping_cl(null)
	,ping_cmdid(0)
{
	try
	{
		timer = null;
		syncdb=SyncDB::instance();
		synconf=Synconf::instance();

		svrhost=synconf->getstr("server_host");
		svrport=synconf->getstr("server_port");
		synconf->setstr("server_host",svrhost);
		synconf->setstr("server_port",svrport);

		QString strTemp=synconf->getstr("heartbeat_interval","10");
		synconf->setstr("heartbeat_interval",strTemp);
		heartbeat_interval=strTemp.toInt();
		strTemp=synconf->getstr("timeout_secs");
		timeout_secs=strTemp.toInt();
		if(timeout_secs==0)
			timeout_secs=2*heartbeat_interval;//默认是2次ping时间就判定超时
		synconf->setstr("timeout_secs",QString::number(timeout_secs),true);

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
	QMutexLocker locker(&m_locker_cmdid);
	quint32 cmdid=synconf->getstr("next_cmdid","1").toInt();
	
	CommandMap mp=syncdb->singleQuerySql("select max(cmd_id) as mid from sqout");
	quint32 maxid=mp["mid"].toInt();
	if(maxid>=cmdid)
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
		svrhost=synconf->getstr("server_host");
		svrport=synconf->getstr("server_port");
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
	if(ping_cl==&client)
		ping_cl=null;
}

void SessionManager::heartbeat()
{
	if(bAutoConnectHost)
	{
		ConnectHost();
	}
	//判断ping包是否超时
	QDateTime qtm=QDateTime::currentDateTime();
	if(ping_acktime.addSecs(timeout_secs) < qtm && bAutoConnectHost)
	{
		//超时，需要重新连接了
		qDebug((ping_acktime.toString()+" "+qtm.toString()).toLocal8Bit().data());
		ping_cl=null;
		DisconnectHost();
		ConnectHost();
	}

	if(++heartbeat_count > heartbeat_interval)
	{
		heartbeat_count=0;
		if(ping_cl && ping_cl->getSocket()->state()==QTcpSocket::ConnectedState)
			say_ping(ping_cl);
	}
	if(ping_cl && ping_cl->getSocket()->state()==QTcpSocket::ConnectedState)
	{
		//处理未发送的命令
		resend_cmd();
		//处理未完成的任务
		dispatch_task();
	}
}

void SessionManager::client_connected(Client *cl)
{
	ping_cl=cl;
	ping_acktime=QDateTime::currentDateTime();
	say_hello(ping_cl);
}

void SessionManager::client_disconnected(Client *cl)
{
	if(ping_cl == cl)
		ping_cl=null;
}

//返回 nCmdID 
int SessionManager::send_data(CommandMap mapCmdLine,QByteArray buffer)
{
	//TODO:还需要一个send_data，来处理本地产生的业务命令，将Client的 say语句都迁移到SessionManager里实现	
	//目前入队列和发送等还是在独立的函数里直接实现的，暂时不处理这个函数
	return 0;
}

/*
recv_data处理进入的命令请求，存入命令队列
send_data
*/
void SessionManager::recv_data(Client *cl,quint32 nCmdID,QString strCmdStr,QString strCmdLine,CommandMap props ,QByteArray buffer)
{
	QString str=QString::fromLocal8Bit("收到命令：%1");
	str=str.arg(strCmdLine);
	qDebug(str.toLocal8Bit().data());
	emit sigLogger(str);

	//除了响应state，其余命令都应该先入队列，后续处理时对于复杂的任务，考虑开线程去处理
	int nCmdType=get_cmdtype(strCmdStr.toLocal8Bit().data());
	if(nCmdType == CMD_UNKNOWN)
	{
		emit sigLogger(QString::fromLocal8Bit("错误！未知命令：")+strCmdLine);
		return ;
	}
	if(nCmdType != CMD_STATE )
	{
		if(syncdb->exist_cmd(props["1"]),QUEUE_IN)
		{
			//收到的命令之前已经收到过，反馈命令重复错误
			emit sigLogger(QString::fromLocal8Bit("收到重复的CMDID：")+strCmdLine);
			CommandMap ack_props;
			ack_props["0"]=get_cmdstr(CMD_STATE);
			ack_props["1"]=props["1"];
			ack_props["2"]=QString::number(STA_CMD_EXEC_FAIL);
			ack_state(cl,ack_props);
			return ;
		}
		//新命令，先添加到数据库里，然后再做本次任务分发
		syncdb->put_cmd(props["1"],strCmdLine,QUEUE_IN);
		do_job(props,cl);
	}
	else
	{
		//收到state的处理
		//从发送队列里找该命令，如果找不到，或者命令已经完成，则丢弃，但是记录日志，否则修改命令为完成
		//如果是PING命令的响应，则不入命令队列，所以应该在成员变量里记录和比较
		if(nCmdType==ping_cmdid)
		{
			ping_acktime=QDateTime::currentDateTime();
			emit sigLogger("ping ack @"+ping_acktime.toString());
			return ;
		}
		if(syncdb->exist_cmd(props["1"],QUEUE_OUT))
		{
			CommandMap old_props=syncdb->get_cmd(nCmdID,QUEUE_OUT);
			if(old_props.size()==0)
			{
				syncdb->tag_cmd(props["1"],TAG_COMPLETE,strCmdLine,QUEUE_OUT);
			}
			else
				emit sigLogger(QString::fromLocal8Bit("收到错误的命令响应A：")+strCmdLine);
		}
		else
			emit sigLogger(QString::fromLocal8Bit("收到错误的命令响应B：")+strCmdLine);
	}
}

// 外发系列协议命令实现
int SessionManager::say_hello(Client *cl)
{
	CommandMap props;
	//hello 101 syncany_client=2.1.0.0 protocol=1.0.1.0 platform=symbian-os-s60.3
	props["syncany_client"]=synconf->getinfo("client_version");
	props["protocol"]=synconf->getinfo("protocol_version");
	props["platform"]=synconf->getinfo("os_version");
	props["0"]=get_cmdstr(CMD_HELLO);
	props["1"]=generate_cmdid();

	QString strCmdLine=convert_to_cmdline(props);
	syncdb->put_cmd(props["1"],strCmdLine,QUEUE_OUT);
	int ret=cl->sendData(strCmdLine);
	if(ret==ERR_NONE)
		syncdb->tag_cmd(props["1"],TAG_SENDING,QUEUE_OUT);
	//else //send fail....?
	//  
	return ret;
}

int SessionManager::say_ping(Client *cl)
{
	QString strCmdLine(get_cmdstr(CMD_PING));
	strCmdLine+=" ";
	strCmdLine+=generate_cmdid();
	strCmdLine+="\n";
	return cl->sendData(strCmdLine);
}

int SessionManager::ack_state(Client *cl,CommandMap props,QByteArray data)
{
	if(data.length()>0 )
	{
		//未指定长度，补充之
		props["size"]=data.length();
	}
	else
	{
		if(props.find("size")!=props.end())
			props.remove("size");
	}
	props["0"]=get_cmdstr(CMD_STATE);

	QString cmdbuf=convert_to_cmdline(props);
	syncdb->tag_cmd(props["1"],TAG_COMPLETE,cmdbuf,QUEUE_IN);
	if(data.length()>0)
		cmdbuf.append(data);
	return cl->sendData(cmdbuf);
}


int SessionManager::ack_bye(Client *cl,CommandMap props)
{
	int ret=ERR_UNKNOWN;
//	if(props.find("1")!=props.end())
	{
		CommandMap ack_props;
		ack_props["1"]=props["1"];
		ack_props["0"]=get_cmdstr(CMD_STATE);
		QString cmdbuf=convert_to_cmdline(props);
		syncdb->tag_cmd(ack_props["1"],TAG_COMPLETE,cmdbuf,QUEUE_IN);
	}
	cl->DisconnectHost();
	return ret;
}

void SessionManager::dispatch_task() //将未完成的任务分法给客户端的其他模块来完成
{
	while(true)
	{
		CommandMap cmdfields=syncdb->singleQuerySql(QString("select cmd_id,cmd_str,cmd_ret from ")+strQueueTableName[QUEUE_IN]+" where tag="+QString::number(TAG_UNSEND)+" limit 1");
		if(cmdfields.size()<1)
			break;
		CommandMap props=convert_from_cmdline(cmdfields["cmd_str"]);
		int nCmdType=get_cmdtype(cmdfields[props[0]].toLocal8Bit().data());
		if(nCmdType==CMD_PUT || nCmdType==CMD_GET||props.find("size")!=props.end())
		{
			//对于一些不适合重做的任务，全部标记为放弃
			syncdb->tag_cmd(props["1"],TAG_ABANDON,QUEUE_IN);
			continue;
		}
		do_job(props,ping_cl);//分发一个待完成的任务
	}
}

void SessionManager::resend_cmd()//将未发送的命令重新发送
{
	while(true)
	{
		CommandMap cmdfields=syncdb->singleQuerySql(QString("select cmd_id,cmd_str,cmd_ret from ")+strQueueTableName[QUEUE_OUT]+" where tag="+QString::number(TAG_UNSEND)+" limit 1");
		if(cmdfields.size()<1)
			break;
		CommandMap props=convert_from_cmdline(cmdfields["cmd_str"]);
		int nCmdType=get_cmdtype(cmdfields[props["0"]].toLocal8Bit().data());
		if(nCmdType==CMD_HELLO || nCmdType==CMD_PUT || nCmdType==CMD_GET||props.find("size")!=props.end())
		{
			//对于一些不适合重做的任务，全部标记为放弃
			syncdb->tag_cmd(props["1"],TAG_ABANDON,QUEUE_OUT);
			continue;
		}
		do_sendcmd(props,ping_cl);//分发一个待完成的任务
	}
}

//处理进入的任务
int SessionManager::do_job(CommandMap props,Client *cl)
{
	QMutexLocker locker(&m_locker);

	int nCmdID=props["1"].toInt();
	syncdb->tag_cmd(props["1"],TAG_SENDING,QUEUE_IN);
	int nCmdType=get_cmdtype(props["0"].toStdString().c_str());
	switch(nCmdType)
	{
	case CMD_BYE:
		{
			if(ack_bye(cl,props)==ERR_NONE)
			{
				emit sigLogger("ack_bye completed!");
			}
			else
			{
				emit sigLogger("ack_bye failed!");
			}
		}
	case CMD_ALERT:
		{
			//TODO：处理注册结果，事件通知——需要向上层冒泡，能处理的自行处理
			emit sigLogger(QString::fromLocal8Bit("收到Alert命令，向上冒泡通知：")+convert_to_cmdline(props));
		}
		break;
	case CMD_WHOAREYOU:
		{
			CommandMap ack_props;
			ack_props["1"]=props["1"];//回应的命令ID
			ack_props["sessionid"]=synconf->getstr("session_id");
			ack_props["user"]=synconf->getstr("user_id");
			ack_props["pwd"]=synconf->getstr("user_password");
			ack_props["devuid"]=synconf->getstr("device_id");
			if(ack_state(cl,ack_props)==ERR_NONE)
			{
				emit sigLogger("ack_whoareyou completed!");
			}
			else
			{
				emit sigLogger("ack_whoareyou failed!");
			}
		}
		break;
	default:
		emit sigLogger(QString::fromLocal8Bit("错误！未知命令：")+convert_to_cmdline(props));
		break;
	}
	return ERR_NONE;
}

//处理外发的命令
int SessionManager::do_sendcmd(CommandMap props,Client *cl)
{
	syncdb->tag_cmd(props["1"],TAG_SENDING,QUEUE_OUT);
	QString strCmdLine=convert_to_cmdline(props);
	return cl->sendData(strCmdLine);
}


