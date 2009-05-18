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
	emit sigLogger("connected!");
	emit connected(this);
}

void Client::disconnected ()
{
	emit sigLogger("disconnected!");
	emit disconnected(this);
}

void Client::error ( QTcpSocket::SocketError socketError )
{
	emit sigLogger(QObject::tr("error:[%1][%2]").arg(socketError).arg(m_sock->errorString()));
}

void Client::hostFound ()
{
	emit sigLogger("hostFound!");
}

void Client::stateChanged ( QTcpSocket::SocketState socketState )
{
	emit sigLogger(QObject::tr("stateChanged!%1").arg(socketState));
}

void Client::readData()
{
	QMutexLocker locker(&m_locker_in);
	buffer.append(m_sock->readAll());
	if(bWaitingCommand)
	{
		while(true)
		{
			if(buffer.size()<1)
				return ;
			int lb=buffer.indexOf('\n');
			if(lb==-1)	return ; //命令必然是以\n结束

			strCmdLine = buffer.left(lb);
			strCmdLine.remove('\r');
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
				emit sigLogger(QObject::tr("Error Command Recv(argument is missed!):%1!").arg(strCmdLine));
				continue;
			}
			bool ok;

			strCmdID=strlist[1];
			int nCmdID=strCmdID.toInt(&ok);

			if(!ok)
			{	
				//错误的命令，丢弃
				emit sigLogger(QObject::tr("Error Command Recv( CmdID is not number!):%1!").arg(strCmdLine));
				continue;
			}

			strCmdStr=strlist[0];
			nCmdType=get_cmdtype(strCmdStr.toStdString().c_str());

			if(nCmdType==CMD_UNKNOWN)
			{
				//错误的命令，丢弃
				emit sigLogger(QObject::tr("Error Command Recv( CmdID is not number!):%1!").arg(strCmdLine));
				continue;
			}
			//提取命令携带的属性
			props=convert_from_cmdline(strCmdLine);
			
			//判断有没有size属性，有则进入等待数据模式，否则将命令发送信号，交给相关的槽去进行处理
			datalen=0;
			if(props.find("size") != props.end())
			{

                datalen=props["size"].toInt(&ok);

				if(!ok)
				{
					emit sigIn(strCmdLine);
					emit sigRecv(this,nCmdID,strCmdStr,strCmdLine,props,QByteArray());
					continue;
				}
				bWaitingCommand=false;//进入等待数据模式
				emit sigIn(strCmdLine);
				break;
			}
			//发送命令信号
			emit sigRecv(this,nCmdID,strCmdStr,strCmdLine,props,QByteArray());
			emit sigIn(strCmdLine);
		}
	}
	
	//这里要考虑缓冲太大了是否记录到文件里
	if(buffer.size()>=datalen)
	{
		QByteArray data=buffer.left(datalen);
		buffer.remove(0,datalen);

		emit sigRecv(this,strCmdID.toInt(),strCmdStr,strCmdLine,props,data);
		bWaitingCommand=true;
	}
	else
	{
		emit sigLogger(QDateTime::currentDateTime().toString()+":"+QString::number(buffer.length()));
	}
	//else 继续等待下一批数据到来
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
		emit sigLogger(QObject::tr("SEND:%1 fail! server is not connected!").arg(str.data()));
		return ERR_NETWORK_CONNECT_FAIL;
	}
}

int Client::sendData(QString str)
{
	return sendData(str.toAscii());
}

