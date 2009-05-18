#include "sessionmanager.h"
#include "syncbasefile.h"

SessionManager::SessionManager(QObject *parent)
	: ITrayMessage(parent)
        ,bAutoConnectHost(false)//���Զ�����
		,ping_cmdid(0)
        ,ping_cl(null)
        ,client(this)
{
	try
	{
		timer = null;
		syncdb=SyncDB::instance();
		synconf=Synconf::instance();

		svrhost=synconf->getstr(KEY_SERVER_IP);
		svrport=synconf->getstr(KEY_SERVER_PORT);
		synconf->setstr(KEY_SERVER_IP,svrhost);
		synconf->setstr(KEY_SERVER_PORT,svrport);

		connect(synconf,SIGNAL(keyChanged(QString)),this,SLOT(ConfigChanged(QString)));

		heartbeat_interval=synconf->getint("heartbeat_interval",1000);
		timeout_secs=synconf->getint("timeout_secs",5);
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
		svrhost=synconf->getstr(KEY_SERVER_IP);
		svrport=synconf->getstr(KEY_SERVER_PORT);
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
	//�ж�ping���Ƿ�ʱ
	QDateTime qtm=QDateTime::currentDateTime();
	if(ping_acktime.addSecs(timeout_secs) < qtm && bAutoConnectHost)
	{
		if(ping_acktime.addSecs(timeout_secs + (heartbeat_interval/1000) *2) < qtm)
		{
			//��ʱ����Ҫ����������
			QString strLog=QObject::tr("last ping ack:")+ping_acktime.toString()+QObject::tr(" current time:")+qtm.toString();
			update_ping_acktime();
			//qDebug(strLog.toLocal8Bit().data());
			emit sigLogger(strLog);
			ping_cl=null;
			DisconnectHost();
			ConnectHost();
		}
		else
		{
			say_ping(ping_cl);
		}
	}


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
	QString str=QObject::tr("recv command:%1");
	str=str.arg(strCmdLine);
	
	update_ping_acktime();
	emit sigLogger(str);


	//������Ӧstate���������Ӧ��������У���������ʱ���ڸ��ӵ����񣬿��ǿ��߳�ȥ����
	int nCmdType=get_cmdtype(strCmdStr.toLocal8Bit().data());
	if(nCmdType == CMD_UNKNOWN)
	{
		emit sigLogger(QObject::tr("error!Unknown command:")+strCmdLine);
		return ;
	}
	if(!buffer.isNull() && buffer.length()>0)
	{
		//TODO�������ѹ�����ݣ�Ӧ��������⿪
		if(props.find("zip")!=props.end())
		{
			buffer=qUncompress(buffer);
		}
		QString strTempFile=strTempDirectory+((nCmdType != CMD_STATE )?"/in/":"/out/");
		QDir().mkpath(strTempFile);
		strTempFile+=props[KEY_CMDID]+".tmp";
		QFile qf(strTempFile);
		qf.open(QIODevice::WriteOnly|QIODevice::Truncate);
		qf.write(buffer);
		qf.close();
	}
	if(nCmdType != CMD_STATE )
	{
		if(nCmdType != CMD_WHOAREYOU)
		{
			if(syncdb->cmd_exist(props[KEY_CMDID]),QUEUE_IN)
			{
				//�յ�������֮ǰ�Ѿ��յ��������������ظ�����
				emit sigLogger(QObject::tr("recv CMDID again!")+strCmdLine);
				CommandMap ack_props;
				ack_props[KEY_CMD]=get_cmdstr(CMD_STATE);
				ack_props[KEY_CMDID]=props[KEY_CMDID];
				ack_props[KEY_STATECODE]=QString::number(STA_CMD_EXEC_FAIL);
				ack_state(cl,ack_props);
				return ;
			}
			//���������ӵ����ݿ��Ȼ��������������ַ�
			syncdb->cmd_put(props[KEY_CMDID],strCmdLine,QUEUE_IN);
		}
		do_job(cl,props,buffer);
	}
	else
	{
		//�յ�state�Ĵ���
		//�ӷ��Ͷ������Ҹ��������Ҳ��������������Ѿ���ɣ����������Ǽ�¼��־�������޸�����Ϊ���
		//�����PING�������Ӧ������������У�����Ӧ���ڳ�Ա�������¼�ͱȽ�
		if(cmd_waiter.contains(props[KEY_CMDID]))
		{
			cmd_waiter.removeOne(props[KEY_CMDID]);
		}
		//ֻҪ�����ݷ��أ�ping���������ʱ������
		if(props[KEY_CMDID].toInt()==ping_cmdid)
		{
			ping_acktime=QDateTime::currentDateTime();
			emit sigLogger("ping ack @"+ping_acktime.toString());
			return ;
		}
		if(syncdb->cmd_exist(props[KEY_CMDID],QUEUE_OUT))
		{
			CommandMap old_props=syncdb->cmd_get(props[KEY_CMDID],QUEUE_OUT);
			if(old_props.size()!=0)
			{
				syncdb->cmd_tag(props[KEY_CMDID],TAG_COMPLETE,strCmdLine,QUEUE_OUT);
				//������Ӧ������������
				do_state(cl,props,old_props,buffer);
			}
			else
				emit sigLogger(QObject::tr("wrong command response A:")+strCmdLine);
		}
		else
			emit sigLogger(QObject::tr("wrong command response B:")+strCmdLine);
	}
}

// �ⷢϵ��Э������ʵ��
int SessionManager::say_hello(Client *cl)
{
	CommandMap props;
	int ret=ERR_NONE;
	//hello 101 syncany_client=2.1.0.0 protocol=1.0.1.0 platform=symbian-os-s60.3

	QString strSessionID=synconf->getstr(KEY_SESSION_ID);
	QString strDeviceID=synconf->getstr(KEY_DEVICE_ID);
	if(strSessionID!="" && strDeviceID!="")
	{
		props[KEY_SESSION_ID]=strSessionID;
		props[KEY_DEVICE_ID]=strDeviceID;
	}
	else
	{
		QString strUsername=synconf->getstr("user_id");
		QString strPassword=synconf->getstr("user_password");
		if(strUsername!="" && strPassword!="")
		{
			props["user"]=strUsername;
			props["pass"]=strPassword;

			if(synconf->getstr("doregister")=="1")
			{
				//������ע�ᣬ�����ǵ�¼
				props[KEY_CMD]=get_cmdstr(CMD_ALERT);
				props[KEY_CMDID]=generate_cmdid();
				props["type"]=ALERT_TYPESTR_SIGNUP;
				QString strCmdLine=convert_to_cmdline(props)+"\n";
				syncdb->cmd_put(props[KEY_CMDID],strCmdLine,QUEUE_OUT);
				CommandMap reg_props=syncSend(cl,props[KEY_CMDID],strCmdLine);
				//ret=cl->sendData(strCmdLine);
				//if(ret==ERR_NONE)
				//	syncdb->cmd_tag(props[KEY_CMDID],TAG_SENDING,"",QUEUE_OUT);
				if(reg_props[KEY_STATECODE].toInt()!=STA_OK)
					return ret;
				//ע��ɹ�����ʼ��¼
				props.clear();
				props["user"]=strUsername;
				props["pass"]=strPassword;
			}
		}
		else
		{
			//δ��дע����Ϣ����hello
			return ret;
		}
	}
	props["syncany_client"]=synconf->getinfo("client_version");
	props["protocol"]=synconf->getinfo("protocol_version");
	props["platform"]=synconf->getinfo("os_version");

	props[KEY_CMD]=get_cmdstr(CMD_HELLO);
	props[KEY_CMDID]=generate_cmdid();

	QString strCmdLine=convert_to_cmdline(props)+"\n";
	syncdb->cmd_put(props[KEY_CMDID],strCmdLine,QUEUE_OUT);
	ret=cl->sendData(strCmdLine);
	if(ret==ERR_NONE)
		syncdb->cmd_tag(props[KEY_CMDID],TAG_SENDING,"",QUEUE_OUT);
	//else //send fail....?
	//  
	return ret;
}

void SessionManager::update_ping_acktime()
{
	ping_acktime=QDateTime::currentDateTime();
}


int SessionManager::say_ping(Client *cl)
{
	if(cl==null || cl->getSocket()->state()!=QTcpSocket::ConnectedState)
		return ERR_UNKNOWN;\
	QDateTime qtm=QDateTime::currentDateTime();
	if(ping_sendtime.addSecs((heartbeat_interval/1000) *2) > qtm) //2�������ڲ����·�
		return ERR_UNKNOWN;
	ping_sendtime=qtm;

	QString strCmdLine(get_cmdstr(CMD_PING));
	strCmdLine+=" ";
	QString strCmdID=generate_cmdid();
	strCmdLine+=strCmdID;
	strCmdLine+="\n";
	ping_cmdid=strCmdID.toInt();

	cmd_waiter.append(strCmdID);
	int ret=ping_cl->sendData(strCmdLine);
	while(true)
	{
		ping_cl->getSocket()->waitForReadyRead(60000);
		if(!cmd_waiter.contains(strCmdID))
			break;
	}

	return ret;
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
	props[KEY_CMD]=get_cmdstr(CMD_STATE);

	QString cmdbuf=convert_to_cmdline(props)+"\n";
	syncdb->cmd_tag(props[KEY_CMDID],TAG_COMPLETE,cmdbuf,QUEUE_IN);
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
		ack_props[KEY_CMDID]=props[KEY_CMDID];
		ack_props[KEY_CMD]=get_cmdstr(CMD_STATE);
		QString cmdbuf=convert_to_cmdline(props)+"\n";
		syncdb->cmd_tag(ack_props[KEY_CMDID],TAG_COMPLETE,cmdbuf,QUEUE_IN);
	}
	cl->DisconnectHost();
	return ret;
}

void SessionManager::dispatch_task() //��δ��ɵ�����ַ����ͻ��˵�����ģ�������
{
	while(true)
	{
                CommandMap cmdfields=syncdb->singleQuerySql(QString("select cmd_id,cmd_str,cmd_ret from ")+TABLE_SQIN+" where tag="+QString::number(TAG_UNSEND)+" limit 1");
		if(cmdfields.size()<1)
			break;
		CommandMap props=convert_from_cmdline(cmdfields["cmd_str"]);
		int nCmdType=get_cmdtype(cmdfields[props[0]].toLocal8Bit().data());
		if(nCmdType==CMD_PUT || nCmdType==CMD_GET||props.find("size")!=props.end())
		{
			//����һЩ���ʺ�����������ȫ�����Ϊ����
			syncdb->cmd_tag(props[KEY_CMDID],TAG_ABANDON,"",QUEUE_IN);
			continue;
		}
		do_job(ping_cl,props);//�ַ�һ������ɵ�����
	}
}

void SessionManager::resend_cmd()//��δ���͵��������·���
{
	while(true)
	{
                CommandMap cmdfields=syncdb->singleQuerySql(QString("select cmd_id,cmd_str,cmd_ret from ")+TABLE_SQOUT+" where tag="+QString::number(TAG_UNSEND)+" limit 1");
		if(cmdfields.size()<1)
			break;
		CommandMap props=convert_from_cmdline(cmdfields["cmd_str"]);
		int nCmdType=get_cmdtype(cmdfields[props[KEY_CMD]].toLocal8Bit().data());
		if(nCmdType==CMD_HELLO || nCmdType==CMD_PUT || nCmdType==CMD_GET||props.find("size")!=props.end())
		{
			//����һЩ���ʺ�����������ȫ�����Ϊ����
			syncdb->cmd_tag(props[KEY_CMDID],TAG_ABANDON,"",QUEUE_OUT);
			continue;
		}
		do_sendcmd(ping_cl,props);//�ַ�һ������ɵ�����
	}
}

//������������
int SessionManager::do_job(Client *cl,CommandMap props,QByteArray data)
{
	QMutexLocker locker(&m_locker);

        //int nCmdID=props[KEY_CMDID].toInt();
	syncdb->cmd_tag(props[KEY_CMDID],TAG_SENDING,"",QUEUE_IN);
	int nCmdType=get_cmdtype(props[KEY_CMD].toStdString().c_str());
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
			//����ALERT���ܴ�������д������ܴ����һ���ϱ����¼�֪ͨ�б�
// #define ALERT_TYPESTR_SINGLE_CHOICE "singlechoice"
// #define ALERT_TYPESTR_MULTI_CHOICE "mutilchoice"
// #define ALERT_TYPESTR_INPUT "input"
// #define ALERT_TYPESTR_REDIRECT "redirect" 
			if(props[KEY_TYPE]==ALERT_TYPESTR_REDIRECT)
			{
				//�յ��������ض���֪ͨ
				QString strHost=props[KEY_SERVER_IP];
				QString strPort=props[KEY_SERVER_PORT];
				synconf->setstr(KEY_SERVER_IP,strHost);
				synconf->setstr(KEY_SERVER_PORT,strPort,true);
				CommandMap ack_props;
				ack_props[KEY_CMDID]=props[KEY_CMDID];//��Ӧ������ID
				ack_props[KEY_STATECODE]=QString::number(STA_OK);
				ack_state(cl,ack_props);
				break;
			}

			if(props[KEY_TYPE]==ALERT_TYPESTR_NOTIFY)
			{
				QString strNotify=QString::fromLocal8Bit(data);
				QStringList strLines=strNotify.split('\n');
				int syncdir=SYNC_FROM_SERVER;
				AlertMessageList msgs;
				for(int i=0;i<strLines.size();++i)
				{
					/*
					���ƽ(81496140)  22:27:20
					alert comid size=***
					version=***  action_type=***  url=***
					version=***  action_type=***  url=***

					kunlun(111345)  22:28:37
					type= ???
					���ƽ(81496140)  22:29:11
					A ��ʾadd
					M ��ʾmodify
					D ��ʾdelete
					*/
					strLines[i].remove('\r');
					int syncop;
					AlertMessage msg;
					CommandMap line_props=convert_from_cmdline(strLines[i]);
					if(line_props[KEY_ACTION_TYPE]!="")
					{
						switch(line_props[KEY_ACTION_TYPE].toLocal8Bit().at(0))
						{
						case 'A':syncop=SYNC_OP_ADD;break;
						case 'M':syncop=SYNC_OP_MODIFY;break;
						case 'D':syncop=SYNC_OP_REMOVE;break;
						case 'N':syncop=SYNC_OP_RENAME;break;
						default: continue;
						}
						msg[KEY_URL]=line_props[KEY_URL];
						msg[KEY_ACTION_TYPE]=QString::number(syncop);
						if(line_props[KEY_VERSION]!="")
							msg[KEY_VERSION]=line_props[KEY_VERSION];
						if(syncop == SYNC_OP_RENAME)
							msg[KEY_URL_NEW]=line_props[KEY_URL_NEW];
					}
				}
				if(msgs.size()>0)
				{
					StringMap msg_props;
					msg_props[KEY_SENDER,"local_file_watcher"];
					msg_props[KEY_VERSION]=props[KEY_VERSION];
					msg_props[KEY_SYNCDIR]=QString::number(SYNC_FROM_SERVER);
					emit alertMessage(msg_props,msgs);
				}
			}
			
			emit sigLogger(QObject::tr("recv Alert Command ,popup it!")+convert_to_cmdline(props));
		}
		break;
	case CMD_WHOAREYOU:
		{
			CommandMap ack_props;
			ack_props[KEY_CMDID]=props[KEY_CMDID];//��Ӧ������ID
			ack_props["sessionid"]=synconf->getstr(KEY_SESSION_ID);
			ack_props["user"]=synconf->getstr("user_id");
			ack_props["pwd"]=synconf->getstr("user_password");
			ack_props["devuid"]=synconf->getstr(KEY_DEVICE_ID);
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
		emit sigLogger(QObject::tr("Error!Unknown Command:")+convert_to_cmdline(props));
		break;
	}
	return ERR_NONE;
}

//�����ⷢ������
int SessionManager::do_sendcmd(Client *cl,CommandMap props,QByteArray data)
{
	syncdb->cmd_tag(props[KEY_CMDID],TAG_SENDING,"",QUEUE_OUT);
	QString strCmdLine=convert_to_cmdline(props)+"\n";
	return cl->sendData(strCmdLine);
}

int SessionManager::do_state(Client *cl,CommandMap state_props,CommandMap cmd_props,QByteArray data)
{
	QString strTmp=convert_to_cmdline(state_props);
	QString strCmd=convert_to_cmdline(cmd_props);
	if(cmd_props[KEY_CMD]==get_cmdstr(CMD_ALERT))
	{
		if(cmd_props["type"]==ALERT_TYPESTR_SIGNUP)
		{
			if(state_props[KEY_STATECODE].toInt()==STA_OK)
			{
				emit trayMessage(QObject::tr("register ok!"),QObject::tr("Your account:")+synconf->getstr("user_id")+QObject::tr(" is registerd!"));
				synconf->setstr(KEY_SESSION_ID,state_props[KEY_SESSION_ID]);
				synconf->setstr(KEY_DEVICE_ID,state_props[KEY_DEVICE_ID]);
				synconf->setstr("doregister","",true);
			}
			else
			{
				QString strReason=state_props[KEY_REASON];
				emit trayMessage(QObject::tr("register fail!"),QObject::tr("Your account:")+synconf->getstr("user_id")+QObject::tr(" regiser failed!errorcode[")+state_props[KEY_STATECODE]+QObject::tr("]!Reason[")+strReason+"]");
			}
		}
		return ERR_NONE;
	}
	else if(cmd_props[KEY_CMD]==get_cmdstr(CMD_HELLO))
	{
		if(cmd_props[KEY_SESSION_ID]!="" || state_props[KEY_SESSION_ID]!="")
		{
			if(state_props[KEY_STATECODE].toInt()==STA_OK)
			{
				emit trayMessage(QObject::tr("Login success!"),QObject::tr("Your account:")+synconf->getstr("user_id")+QObject::tr(" login ok!"));
				if(state_props[KEY_SESSION_ID]!="")
				{
					synconf->setstr(KEY_SESSION_ID,state_props[KEY_SESSION_ID]);
					synconf->setstr(KEY_DEVICE_ID,state_props[KEY_DEVICE_ID],true);
				}
			}
			else
			{				
				QString strReason=state_props[KEY_REASON];
				emit trayMessage(QObject::tr("Login fail!"),QObject::tr("Your account:")+synconf->getstr("user_id")+QObject::tr(" login failed!errorcode[")+state_props[KEY_STATECODE]+QObject::tr("]!Reason[")+strReason+"]");
			}
		}
		else
		{
			QString strReason=state_props[KEY_REASON];
			emit trayMessage(QObject::tr("connect ok!"),QObject::tr("You'v connected server,but you are not logined!"));
		}
		return ERR_NONE;
	}
	else if(cmd_props[KEY_CMD]==get_cmdstr(CMD_PUT))
	{
		emit trayMessage(tr("Item sync over"),tr("The local item ")+cmd_props[KEY_URL]+tr(" has been uploaded successful!"));
	}
	else if(cmd_props[KEY_CMD]==get_cmdstr(CMD_PUT))
	{
		emit trayMessage(tr("Item sync over"),tr("The remote item ")+cmd_props[KEY_URL]+tr(" has been downloaded successful!"));
	}
	//just for debug watch!
	return 0;
}


//����Զ�̵�ַ����ȡ�ļ���Ϣ

CommandMap  SessionManager::syncSend(Client *cl,QString strCmdID,QString strCmdLine,QByteArray buffer)
{
	if(cl==null)
		return CommandMap();
	cmd_waiter.append(strCmdID);
	cl->sendData(strCmdLine);
	if(buffer.size()>0)//����Ҫ��֤buffer�ĳߴ����Ҫ�󣬷���ᵼ��Э��ջ��������
	{
		int ret=cl->sendData(buffer);
		if(ret==ERR_NONE)
			syncdb->cmd_tag(strCmdID,TAG_SENDING,"",QUEUE_OUT);
	}
	int cnt=0;
	while(true)
	{
		cl->getSocket()->waitForReadyRead(60000);
		if(!cmd_waiter.contains(strCmdID))
			break;
		++cnt;
		if(cnt > 2)
			return CommandMap();
	}
	//ȡ����Ӧ���ݷ��أ�buffer�أ���
	CommandMap props=syncdb->cmd_get_ret(strCmdID,QUEUE_OUT);
	return props;
}

QList<PtrFile> SessionManager::ls_file(QString strUrl)
{
	if(ping_cl == null)
		return QList<PtrFile>();

	CommandMap props;
	props["url"]=strUrl;
	props[KEY_CMD]=get_cmdstr(CMD_LS);
	QString strCmdID=generate_cmdid();
	props[KEY_CMDID]=strCmdID;

	QString strCmdLine=convert_to_cmdline(props)+"\n";
	syncdb->cmd_put(strCmdID,strCmdLine,QUEUE_OUT);

	cmd_waiter.append(strCmdID);
	int ret=ping_cl->sendData(strCmdLine);
	while(true)
	{
		ping_cl->getSocket()->waitForReadyRead(60000);
		if(!cmd_waiter.contains(strCmdID))
			break;
	}
	QString strTempFile=strTempDirectory+"/out/"+strCmdID+".tmp";
	QFile qf(strTempFile);
	qf.open(QIODevice::ReadOnly);
	QByteArray buffer=qf.readAll();
	qf.close();
	if(ret==ERR_NONE)
		syncdb->cmd_tag(props[KEY_CMDID],TAG_SENDING,"",QUEUE_OUT);
	QList<PtrFile> ptrfiles;
	QString strBuffer=buffer;
	QStringList strList=strBuffer.split(' ');
	for(int i=0;i<strList.size();++i)
	{
		if(strList[i].length()==0)
			continue;
		CommandMap props=convert_from_cmdline(strList[i]);
		/*filename	���ش����ļ�������ȫ·����
			filesize	�����ļ���С
			url	���URL��ʾ
			anchor ê�㣨������������
			anchor_time ê���Ӧ������޸�ʱ�䣬���ش洢���ڸ���ê��ʱ����
			modify_time ����޸�ʱ��*/
		QString strType=props["type"];
		PtrFile ptrfile=createFileObject(strType);
		ptrfile->setUrl(props["url"]);
		if(ptrfile->getType()==TYPE_FILE)
		{
			ptrfile->setSize(props["filesize"].toInt());
			ptrfile->setAnchor(props["anchor"].toInt());
		}
		ptrfiles.push_back(ptrfile);
	}
	return ptrfiles;
}
quint32 SessionManager::put_file(PtrFile pf)
{

	if(ping_cl == null)
                return ERR_UNKNOWN;

	QByteArray buf=pf->getData();

	CommandMap props;
	props["url"]=pf->getUrl();
	props["size"]=QString::number(buf.length());
	props[KEY_CMD]=get_cmdstr(CMD_PUT);
	props[KEY_CMDID]=generate_cmdid();

	QString strCmdLine=convert_to_cmdline(props)+"\n";
	syncdb->cmd_put(props[KEY_CMDID],strCmdLine,QUEUE_OUT);
	
	int ret=ping_cl->sendData(strCmdLine);
	if(ret==ERR_NONE)
		syncdb->cmd_tag(props[KEY_CMDID],TAG_SENDING,"",QUEUE_OUT);
	return ret;

}

quint32 SessionManager::rm_file(PtrFile pf)
{
	if(ping_cl == null)
                return ERR_UNKNOWN;


	CommandMap props;
	props["url"]=pf->getUrl();
	props[KEY_CMD]=get_cmdstr(CMD_RM);
	props[KEY_CMDID]=generate_cmdid();

	QString strCmdLine=convert_to_cmdline(props)+"\n";
	syncdb->cmd_put(props[KEY_CMDID],strCmdLine,QUEUE_OUT);

	int ret=ping_cl->sendData(strCmdLine);
	if(ret==ERR_NONE)
		syncdb->cmd_tag(props[KEY_CMDID],TAG_SENDING,"",QUEUE_OUT);
	return ret;


}


quint32 SessionManager::get_file(QString strUrl)
{

	if(ping_cl == null)
		return ERR_UNKNOWN;


	CommandMap props;
	props["url"]=strUrl;
	props[KEY_CMD]=get_cmdstr(CMD_GET);
	props[KEY_CMDID]=generate_cmdid();

	QString strCmdLine=convert_to_cmdline(props)+"\n";
	syncdb->cmd_put(props[KEY_CMDID],strCmdLine,QUEUE_OUT);

	int ret=ping_cl->sendData(strCmdLine);
	if(ret==ERR_NONE)
		syncdb->cmd_tag(props[KEY_CMDID],TAG_SENDING,"",QUEUE_OUT);
	return ret;

}

void SessionManager::ConfigChanged(QString key)
{
	if(key==KEY_SERVER_IP)
		svrhost=synconf->getstr(key);

	if(key==KEY_SERVER_PORT)
		svrhost=synconf->getstr(key);

	if(key=="heartbeat_interval")
		heartbeat_interval=synconf->getint(key);

	if(key=="timeout_secs")
		timeout_secs=synconf->getint(key);

	if(key=="temp_path")
		strTempDirectory=synconf->getstr("temp_path");
}


