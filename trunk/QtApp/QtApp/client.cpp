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
	QMutexLocker locker(&m_locker_out);//必须放这里，否则锁重入
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
	QMutexLocker locker(&m_locker_out);
	buffer+=m_sock->readAll();
	if(bWaitingCommand)
	{
		while(true)
		{
			if(buffer.size()<1)
				return ;
			int lb=buffer.indexOf('\n');
			if(lb==-1)	return ; //命令必然是以\n结束

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
				//错误的命令，丢弃
				emit sigLogger(QString("Error Command Recv(argument is missed!):%1!").arg(strCmdLine));
				continue;
			}
			bool ok;

			strCmdID=strlist[1];
			int nCmdID=strCmdID.toInt(&ok);

			if(!ok)
			{	
				//错误的命令，丢弃
				emit sigLogger(QString("Error Command Recv( CmdID is not number!):%1!").arg(strCmdLine));
				continue;
			}

			strCmdStr=strlist[0];
			nCmdType=get_cmdtype(strCmdStr.toStdString().c_str());

			if(nCmdType==CMD_UNKNOWN)
			{
				//错误的命令，丢弃
				emit sigLogger(QString("Error Command Recv( CmdID is not number!):%1!").arg(strCmdLine));
				continue;
			}
			//提取命令携带的属性
			props.clear();
			int id=0;
			for(int i=0;i<strlist.size();++i)
			{
				QStringList listKeyValue=strlist[i].split('=');
				switch(listKeyValue.size()) //对于非=号的单独属性序列，使用数字序号来代表KEY
				{
				case 1:
					props[QString(id++)]=listKeyValue[0];
					break;
				case 2:
					props[listKeyValue[0]]=listKeyValue[1];
					break;
				default:
					continue;
				}
			}
			//判断有没有size属性，有则进入等待数据模式，否则将命令发送信号，交给相关的槽去进行处理
			datalen=0;
			if(props.find("size") != props.end())
			{

				int datalen=props["size"].toInt(&ok);

				if(!ok)
				{
					emit sigRecv(this,nCmdID,strCmdStr,strCmdLine,props,QByteArray());
					continue;
				}
				bWaitingCommand=false;//进入等待数据模式
				emit sigIn(strCmdLine);
				break;
			}
			//发送命令信号
			emit sigRecv(this,nCmdID,strCmdStr,strCmdLine,props,QByteArray());
		}
	}
	
	//这里要考虑缓冲太大了是否记录到文件里
	if(buffer.size()>=datalen)
	{
		QByteArray data=buffer.left(datalen);
		emit sigRecv(this,nCmdID,strCmdStr,strCmdLine,props,data);
		bWaitingCommand=true;
	}
	//else 继续等待下一批数据到来
}

void Client::sendData(QString str)
{
	if(m_sock && m_sock->state()==QTcpSocket::ConnectedState)
	{
		QMutexLocker locker(&m_locker_out);
		m_sock->write(str.toAscii());
		emit sigOut(str);
	}
	else
		emit sigLogger(QString("SEND:%s fail! server is not connected!").arg(str));
}


void Client::say_ping(QString strCmdID)
{
	if(m_sock && m_sock->state()==QTcpSocket::ConnectedState)
	{
		QString strCmdStr("ping ");
		strCmdStr+=strCmdID;
		strCmdStr+="\n";
		sendData(strCmdStr);
	}
}

void Client::say_hello(QString strCmdID,QMap<QString,QString> props)
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
		sendData(strCmdStr);
	}
}

void Client::ack_bye(QMap<QString,QString> props)
{
	if(props.find("1")!=props.end())
	{
		QString strCmdID=props["1"];
		if(strCmdID!=0 && strCmdID.toInt()!=0)
		{
			QString strCmdStr="state "+strCmdID+" 200\n";
			sendData(strCmdStr);
			m_sock->waitForBytesWritten(100);
		}
	}
	m_sock->DisconnectHost();
}

void Client::ack_state(QMap<QString,QString> props)
{
}


void Client::ack_state(QMap<QString,QString> props,QByteArray data)
{
}

