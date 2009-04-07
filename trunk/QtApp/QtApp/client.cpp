#include "client.h"

Client::Client(QObject *parent)
	: QObject(parent)
{
	/*
	void connected ()
	void disconnected ()
	void error ( QAbstractSocket::SocketError socketError )
	void hostFound ()
	void stateChanged ( QAbstractSocket::SocketState socketState )
	*/
	connect(&m_sock,SIGNAL(connected()),this,SLOT(connected()))
}

Client::~Client()
{

}

void Client::connected ()
{

}

void Client::disconnected ()
{

}

void Client::error ( QAbstractSocket::SocketError socketError )
{

}

void Client::hostFound ()
{

}

void Client::stateChanged ( QAbstractSocket::SocketState socketState )
{

}

