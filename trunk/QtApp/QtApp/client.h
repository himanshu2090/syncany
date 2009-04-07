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
//自定义一个信号，用于输出日志

private:
	QTcpSocket m_sock;
};

#endif // CLIENT_H
