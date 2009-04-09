#include "client.h"

Client::Client(QObject *parent)
	: QObject(parent)
{
	m_sock=0;
}

Client::~Client()
{
	if(m_sock!=0)
		delete m_sock;
}

void Client::connectSignal()
{
	connect(m_sock,SIGNAL(connected()),this,SLOT(connected()));
	connect(m_sock,SIGNAL(disconnected()),this,SLOT(disconnected()));
	connect(m_sock,SIGNAL(readyRead()), this, SLOT(readData()));
	connect(m_sock,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(error(QAbstractSocket::SocketError)));
	connect(m_sock,SIGNAL(stateChanged(QAbstractSocket::SocketError)),this,SLOT(stateChanged(QAbstractSocket::SocketError)));
}

void Client::ConnectHost(QString host,quint32 port)
{
	if(m_sock!=0)
	{
		if(m_sock->state()!=QTcpSocket::UnconnectedState)
			m_sock->disconnectFromHost();
		delete m_sock;
		m_sock=0;
	}
	if(m_sock==0)
		m_sock=new QTcpSocket();
	connectSignal();
	m_sock->connectToHost(host,port);
}

void Client::DisconnectHost()
{
	if(m_sock!=0)
	{
		m_sock->disconnectFromHost();
	}

}


void Client::connected ()
{
	emit sigLogger("connected!");
}

void Client::disconnected ()
{
	emit sigLogger("disconnected!");
}

void Client::error ( QTcpSocket::SocketError socketError )
{
	emit sigLogger(QString("error:[%1][%s]").arg(socketError).arg(m_sock->errorString()));
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
		m_sock->write(str.toAscii());
		emit sigOut(str);
	}
	else
		emit sigLogger(QString("SEND:%s fail! server is not connected!").arg(str));
}