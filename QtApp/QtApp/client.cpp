#include "client.h"

Client::Client(QObject *parent)
	: QObject(parent)
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
	m_sock->connectToHost(host,port);
}

void Client::DisconnectHost()
{
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
	QString str=m_sock->readAll();
	emit sigIn(str);
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
	QString strCmdStr="ping "+strCmdID+"\n";
	sendData(strCmdStr);
}

void Client::say_hello(QString strCmdID,QMap<QString,QString> props)
{
	QString strCmdStr="hello "+strCmdID;
	QMap<QString,QString>::const_iterator it=props.constBegin();
	while(it!=props.end())
	{
		strCmdStr+=" "+it.key()+"="+it.value();
		++it;
	}
	strCmdStr+="\n";
	sendData(strCmdStr);
}