#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
//#include <QTcpSocket>

class Client : public QObject
{
	Q_OBJECT

public:
	Client(QObject *parent);
	~Client();
public slots:
	void connected ()
	void disconnected ()
	void error ( QAbstractSocket::SocketError socketError )
	void hostFound ()
	void stateChanged ( QAbstractSocket::SocketState socketState )
//�Զ���һ���źţ����������־

private:
	QTcpSocket m_sock;
};

#endif // CLIENT_H
