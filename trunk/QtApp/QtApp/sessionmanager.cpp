#include "sessionmanager.h"
#include "syncbasefile.h"

SessionManager::SessionManager(QObject *parent)
	: QObject(parent)
	,heartbeat_count(0)
	,bAutoConnectHost(false)//���Զ�����
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

		QString strTemp=synconf->getstr("heartbeat_interval","1000");
		synconf->setstr("heartbeat_interval",strTemp);
		heartbeat_interval=strTemp.toInt();
		strTemp=synconf->getstr("timeout_secs","5");
		timeout_secs=strTemp.toInt();
		synconf->setstr("timeout_secs",QString::number(timeout_secs),true);
		strTempDirectory=synconf->getstr("temp_path","c:/temp/");

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

		syncdb->cmd_reset_queue();

		//�������������Ӧ��SM���Ƚ�������client����

		timer = new QTimer(this);
		connect(timer, SIGNAL(timeout()), this, SLOT(heartbeat()));
		timer->start(heartbeat_interval);
		
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
	syncdb->cmd_reset_queue();
}

//��������ID
QString SessionManager::generate_cmdid()
{
	QMutexLocker locker(&m_locker_cmdid);
	quint32 cmdid=synconf->getstr("next_cmdid","1").toInt();
	
	CommandMap mp=syncdb->singleQuerySql("select max(cmd_id) as mid from sqout");
	quint32 maxid=mp["mid"].toInt();
	if(maxid>=cmdid)
		cmdid=maxid+1;
	synconf->setstr("next_cmdid",QString::number(cmdid+1));
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
	client.sendData("bye "+generate_cmdid().toLocal8Bit()+"\n");
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
#if 0
	//�ж�ping���Ƿ�ʱ
	QDateTime qtm=QDateTime::currentDateTime();
	if(ping_acktime.addSecs(timeout_secs+10) < qtm && bAutoConnectHost)
	{
		//��ʱ����Ҫ����������
		QString strLog=QString::fromLocal8Bit("�ϴ�Ӧ��ping����")+ping_acktime.toString()+QString::fromLocal8Bit(" ��ǰʱ�䣺")+qtm.toString();
		qDebug(strLog.toLocal8Bit().data());
		emit sigLogger(strLog);
		ping_cl=null;
		DisconnectHost();
		ConnectHost();
	}

	if(++heartbeat_count > timeout_secs)
	{
		heartbeat_count=0;
		if(ping_cl && ping_cl->getSocket()->state()==QTcpSocket::ConnectedState)
			say_ping(ping_cl);
	}
#endif
	if(ping_cl && ping_cl->getSocket()->state()==QTcpSocket::ConnectedState)
	{
		//����δ���͵�����
		resend_cmd();
		//����δ��ɵ�����
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

//���� nCmdID 
int SessionManager::send_data(CommandMap mapCmdLine,QByteArray buffer)
{
	//TODO:����Ҫһ��send_data���������ز�����ҵ�������Client�� say��䶼Ǩ�Ƶ�SessionManager��ʵ��	
	//Ŀǰ����кͷ��͵Ȼ����ڶ����ĺ�����ֱ��ʵ�ֵģ���ʱ�������������
	return 0;
}

/*
recv_data���������������󣬴����������
send_data
*/
void SessionManager::recv_data(Client *cl,quint32 nCmdID,QString strCmdStr,QString strCmdLine,CommandMap props ,QByteArray buffer)
{
	QString str=QString::fromLocal8Bit("�յ����%1");
	str=str.arg(strCmdLine);
	
	emit sigLogger(str);


	//������Ӧstate���������Ӧ��������У���������ʱ���ڸ��ӵ����񣬿��ǿ��߳�ȥ����
	int nCmdType=get_cmdtype(strCmdStr.toLocal8Bit().data());
	if(nCmdType == CMD_UNKNOWN)
	{
		emit sigLogger(QString::fromLocal8Bit("����δ֪���")+strCmdLine);
		return ;
	}
	if(!buffer.isNull() && buffer.length()>0)
	{
		QString strTempFile=strTempDirectory+((nCmdType != CMD_STATE )?"/in":"/out")+props["1"]+".tmp";
		QFile qf(strTempDirectory);
		qf.open(QIODevice::Truncate);
		qf.write(buffer);
		qf.close();
	}
	if(nCmdType != CMD_STATE )
	{
		if(nCmdType != CMD_WHOAREYOU)
		{
			if(syncdb->cmd_exist(props["1"]),QUEUE_IN)
			{
				//�յ�������֮ǰ�Ѿ��յ��������������ظ�����
				emit sigLogger(QString::fromLocal8Bit("�յ��ظ���CMDID��")+strCmdLine);
				CommandMap ack_props;
				ack_props["0"]=get_cmdstr(CMD_STATE);
				ack_props["1"]=props["1"];
				ack_props["2"]=QString::number(STA_CMD_EXEC_FAIL);
				ack_state(cl,ack_props);
				return ;
			}
			//���������ӵ����ݿ��Ȼ��������������ַ�
			syncdb->cmd_put(props["1"],strCmdLine,QUEUE_IN);
		}
		do_job(cl,props,buffer);
	}
	else
	{
		//�յ�state�Ĵ���
		//�ӷ��Ͷ������Ҹ��������Ҳ��������������Ѿ���ɣ����������Ǽ�¼��־�������޸�����Ϊ���
		//�����PING�������Ӧ������������У�����Ӧ���ڳ�Ա�������¼�ͱȽ�
		if(props["1"].toInt()==ping_cmdid)
		{
			ping_acktime=QDateTime::currentDateTime();
			emit sigLogger("ping ack @"+ping_acktime.toString());
			return ;
		}
		if(syncdb->cmd_exist(props["1"],QUEUE_OUT))
		{
			if(cmd_waiter.find(props["1"])!=cmd_waiter.end())
			{
				QWaitCondition *qwc=cmd_waiter[props["1"]];
				qwc->wakeAll();
			}
			CommandMap old_props=syncdb->cmd_get(props["1"],QUEUE_OUT);
			if(old_props.size()!=0)
			{
				syncdb->cmd_tag(props["1"],TAG_COMPLETE,strCmdLine,QUEUE_OUT);
				//������Ӧ������������
				do_state(cl,props,buffer);
			}
			else
				emit sigLogger(QString::fromLocal8Bit("�յ������������ӦA��")+strCmdLine);
		}
		else
			emit sigLogger(QString::fromLocal8Bit("�յ������������ӦB��")+strCmdLine);
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
	props["0"]=get_cmdstr(CMD_HELLO);
	props["1"]=generate_cmdid();

	QString strCmdLine=convert_to_cmdline(props)+"\n";
	syncdb->cmd_put(props["1"],strCmdLine,QUEUE_OUT);
	int ret=cl->sendData(strCmdLine);
	if(ret==ERR_NONE)
		syncdb->cmd_tag(props["1"],TAG_SENDING,"",QUEUE_OUT);
	//else //send fail....?
	//  
	return ret;
}

int SessionManager::say_ping(Client *cl)
{
	QString strCmdLine(get_cmdstr(CMD_PING));
	strCmdLine+=" ";
	QString strCmdID=generate_cmdid();
	strCmdLine+=strCmdID;
	strCmdLine+="\n";
	ping_cmdid=strCmdID.toInt();
	return cl->sendData(strCmdLine);
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

	QString cmdbuf=convert_to_cmdline(props)+"\n";
	syncdb->cmd_tag(props["1"],TAG_COMPLETE,cmdbuf,QUEUE_IN);
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
		QString cmdbuf=convert_to_cmdline(props)+"\n";
		syncdb->cmd_tag(ack_props["1"],TAG_COMPLETE,cmdbuf,QUEUE_IN);
	}
	cl->DisconnectHost();
	return ret;
}

void SessionManager::dispatch_task() //��δ��ɵ�����ַ����ͻ��˵�����ģ�������
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
			//����һЩ���ʺ�����������ȫ�����Ϊ����
			syncdb->cmd_tag(props["1"],TAG_ABANDON,"",QUEUE_IN);
			continue;
		}
		do_job(ping_cl,props);//�ַ�һ������ɵ�����
	}
}

void SessionManager::resend_cmd()//��δ���͵��������·���
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
			//����һЩ���ʺ�����������ȫ�����Ϊ����
			syncdb->cmd_tag(props["1"],TAG_ABANDON,"",QUEUE_OUT);
			continue;
		}
		do_sendcmd(ping_cl,props);//�ַ�һ������ɵ�����
	}
}

//������������
int SessionManager::do_job(Client *cl,CommandMap props,QByteArray data)
{
	QMutexLocker locker(&m_locker);

	int nCmdID=props["1"].toInt();
	syncdb->cmd_tag(props["1"],TAG_SENDING,"",QUEUE_IN);
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
			//TODO������ע�������¼�֪ͨ������Ҫ���ϲ�ð�ݣ��ܴ�������д���
			emit sigLogger(QString::fromLocal8Bit("�յ�Alert�������ð��֪ͨ��")+convert_to_cmdline(props));
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
		emit sigLogger(QString::fromLocal8Bit("����δ֪���")+convert_to_cmdline(props));
		break;
	}
	return ERR_NONE;
}

//�����ⷢ������
int SessionManager::do_sendcmd(Client *cl,CommandMap props,QByteArray data)
{
	syncdb->cmd_tag(props["1"],TAG_SENDING,"",QUEUE_OUT);
	QString strCmdLine=convert_to_cmdline(props)+"\n";
	return cl->sendData(strCmdLine);
}

int SessionManager::do_state(Client *cl,CommandMap props,QByteArray data)
{
	return 0;
}

//����Զ�̵�ַ����ȡ�ļ���Ϣ
PtrFile SessionManager::ls_file(QString strUrl)
{
	if(ping_cl == null)
		return PtrFile();

	PtrFile pf=new SyncBaseFile();
	CommandMap props;
	props["url"]=pf->getUrl();
	props["0"]=get_cmdstr(CMD_LS);
	props["1"]=generate_cmdid();

	QString strCmdLine=convert_to_cmdline(props)+"\n";
	syncdb->cmd_put(props["1"],strCmdLine,QUEUE_OUT);

	QWaitCondition qwc;
	QMutex mtx;
	mtx.lock();
	int ret=ping_cl->sendData(strCmdLine);
	cmd_waiter[props["1"]]=&qwc;
	qwc.wait(&mtx);//�᲻����������������

	if(ret==ERR_NONE)
		syncdb->cmd_tag(props["1"],TAG_SENDING,"",QUEUE_OUT);
	//�ȴ�����
	//TODO:

	return PtrFile();
}
quint32 SessionManager::put_file(PtrFile pf)
{

	if(ping_cl == null)
		return -1;

	QByteArray buf=pf->getData();

	CommandMap props;
	props["url"]=pf->getUrl();
	props["size"]=QString::number(buf.length());
	props["0"]=get_cmdstr(CMD_PUT);
	props["1"]=generate_cmdid();

	QString strCmdLine=convert_to_cmdline(props)+"\n";
	syncdb->cmd_put(props["1"],strCmdLine,QUEUE_OUT);
	
	int ret=ping_cl->sendData(strCmdLine);
	if(ret==ERR_NONE)
		syncdb->cmd_tag(props["1"],TAG_SENDING,"",QUEUE_OUT);
	return ret;

}

quint32 SessionManager::rm_file(PtrFile pf)
{
	if(ping_cl == null)
		return -1;


	CommandMap props;
	props["url"]=pf->getUrl();
	props["0"]=get_cmdstr(CMD_RM);
	props["1"]=generate_cmdid();

	QString strCmdLine=convert_to_cmdline(props)+"\n";
	syncdb->cmd_put(props["1"],strCmdLine,QUEUE_OUT);

	int ret=ping_cl->sendData(strCmdLine);
	if(ret==ERR_NONE)
		syncdb->cmd_tag(props["1"],TAG_SENDING,"",QUEUE_OUT);
	return ret;


}


