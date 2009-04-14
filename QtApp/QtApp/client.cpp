#include "client.h"

Client::Client(QObject *parent)
	: QObject(parent)
	,bWaitingCommand(true)
{
	m_sock=new QTcpSocket();
	connectSignal();
}

Client::~Client()
{
	if(m_sock!=0)
	{
		DisconnectHost();
		delete m_sock;
	}
}

void Client::connectSignal()
{
	connect(m_sock,SIGNAL(connected()),this,SLOT(connected()));
	connect(m_sock,SIGNAL(disconnected()),this,SLOT(disconnected()));
	connect(m_sock,SIGNAL(readyRead()), this, SLOT(readData()));
	connect(m_sock,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(error(QAbstractSocket::SocketError)));
	connect(m_sock,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(stateChanged(QAbstractSocket::SocketState)));
}

void Client::ConnectHost(QString host,quint32 port)
{
	DisconnectHost();
	QMutexLocker locker(&m_locker_out);//������������������
	m_sock->connectToHost(host,port);
}

void Client::DisconnectHost()
{
	QMutexLocker locker(&m_locker_out);
	if(m_sock->state()!=QTcpSocket::UnconnectedState)
		m_sock->disconnectFromHost();
} 


void Client::connected ()
{
	emit connected(this);
	emit sigLogger("connected!");
}

void Client::disconnected ()
{
	emit disconnected(this);
	emit sigLogger("disconnected!");
}

void Client::error ( QTcpSocket::SocketError socketError )
{
	emit sigLogger(QString("error:[%1][%2]").arg(socketError).arg(m_sock->errorString()));
}

void Client::hostFound ()
{
	emit sigLogger("hostFound!");
}

void Client::stateChanged ( QTcpSocket::SocketState socketState )
{
	emit sigLogger(QString("stateChanged!%1").arg(socketState));
}

void Client::readData()
{
	QMutexLocker locker(&m_locker_in);
	buffer+=m_sock->readAll();
	if(bWaitingCommand)
	{
		while(true)
		{
			if(buffer.size()<1)
				return ;
			int lb=buffer.indexOf('\n');
			if(lb==-1)	return ; //�����Ȼ����\n����

			strCmdLine = buffer.left(lb);
			//QByteArray bt = buffer.left(lb);
			////strCmdLine.fromAscii(bt.constData());
			//strCmdLine=bt;
			//strCmdLine=bt.data();
			//char * p=bt.data();
			//strCmdLine=p;

			buffer.remove(0,lb+1);
			QStringList strlist=strCmdLine.split(' ');
			if(strlist.size()<2)
			{
				//������������
				emit sigLogger(QString("Error Command Recv(argument is missed!):%1!").arg(strCmdLine));
				continue;
			}
			bool ok;

			strCmdID=strlist[1];
			int nCmdID=strCmdID.toInt(&ok);

			if(!ok)
			{	
				//������������
				emit sigLogger(QString("Error Command Recv( CmdID is not number!):%1!").arg(strCmdLine));
				continue;
			}

			strCmdStr=strlist[0];
			nCmdType=get_cmdtype(strCmdStr.toStdString().c_str());

			if(nCmdType==CMD_UNKNOWN)
			{
				//������������
				emit sigLogger(QString("Error Command Recv( CmdID is not number!):%1!").arg(strCmdLine));
				continue;
			}
			//��ȡ����Я��������
			props=convert_from_cmdline(strCmdLine);
			
			//�ж���û��size���ԣ��������ȴ�����ģʽ������������źţ�������صĲ�ȥ���д���
			datalen=0;
			if(props.find("size") != props.end())
			{

				int datalen=props["size"].toInt(&ok);

				if(!ok)
				{
					emit sigRecv(this,nCmdID,strCmdStr,strCmdLine,props,QByteArray());
					continue;
				}
				bWaitingCommand=false;//����ȴ�����ģʽ
				emit sigIn(strCmdLine);
				break;
			}
			//���������ź�
			emit sigRecv(this,nCmdID,strCmdStr,strCmdLine,props,QByteArray());
		}
	}
	
	//����Ҫ���ǻ���̫�����Ƿ��¼���ļ���
	if(buffer.size()>=datalen)
	{
		QByteArray data=buffer.left(datalen);
		buffer.remove(0,datalen);

		emit sigRecv(this,strCmdID.toInt(),strCmdStr,strCmdLine,props,data);
		bWaitingCommand=true;
	}
	//else �����ȴ���һ�����ݵ���
}

int Client::sendData(QByteArray str)
{
	if(m_sock && m_sock->state()==QTcpSocket::ConnectedState)
	{
		QMutexLocker locker(&m_locker_out);
		emit sigOut(str);
		if(m_sock->write(str.data())!=-1)
		{
			return ERR_NONE;
		}
		else
		{
			return ERR_NETWORK_SEND_FAIL;
		}
	}
	else
	{
		emit sigLogger(QString("SEND:%1 fail! server is not connected!").arg(str.data()));
		return ERR_NETWORK_CONNECT_FAIL;
	}
}

int Client::sendData(QString str)
{
	return sendData(str.toAscii());
}


int Client::say_ping(QString strCmdID)
{
	if(m_sock && m_sock->state()==QTcpSocket::ConnectedState)
	{
		QString strCmdStr("ping ");
		strCmdStr+=strCmdID;
		strCmdStr+="\n";
		return sendData(strCmdStr);
	}
	return ERR_NETWORK_CONNECT_FAIL;
}

int Client::say_hello(QString strCmdID,CommandMap props)
{
	if(m_sock && m_sock->state()==QTcpSocket::ConnectedState)
	{
		QString strCmdStr("hello ");
		strCmdStr+=strCmdID;

		QList<QString> keys=props.keys();

		for(int i=0;i<keys.size();++i)
		{
			strCmdStr+=" ";
			strCmdStr+=keys[i];
			strCmdStr+="=";
			strCmdStr+=props[keys[i]];
		}

		strCmdStr+="\n";
		return sendData(strCmdStr);
	}
	return ERR_NETWORK_CONNECT_FAIL;
}

int Client::ack_bye(CommandMap props)
{
	int ret=ERR_UNKNOWN;

	if(props.find("1")!=props.end())
	{
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

int Client::ack_state(CommandMap props)
{
	return ack_state(props,QByteArray());
}

int Client::ack_state(CommandMap props,QByteArray data)
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

	QByteArray cmdbuf="state "+props["1"].toLocal8Bit();
	QList<QString> keys=props.keys();
	for(int i=0;i<keys.size();++i)
	{
		int nu=keys[i].toInt();
		if(nu>2)
		{
			cmdbuf.append(" ");
			cmdbuf.append(keys[i]);
			continue;
		}
		cmdbuf.append(" ");
		cmdbuf.append(keys[i]);
		cmdbuf.append("=");
		cmdbuf.append(props[keys[i]]);
	}
	cmdbuf.append("\n");
	if(data.length()>0)
		cmdbuf.append(data);
	return sendData(cmdbuf);
}

