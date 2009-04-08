#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>

class Client : public QObject
{
	Q_OBJECT

public:
	Client(QObject *parent);
	~Client();
public slots:
	void connected ();
	void disconnected ();
	void readData();
	void error ( QTcpSocket::SocketError socketError );
	void hostFound ();
	void stateChanged ( QTcpSocket::SocketState socketState );
//自定义一个信号，用于输出日志
signals:
	void explains(); //如果要自定义槽和信号, explains信号是必须的
	void sigOut(QString str);  //自定义信号.
	void sigIn(QString str);  //自定义信号.
	void sigLogger(QString str);  //自定义信号.

public:
	QTcpSocket *getSocket(){return m_sock;}
	void ConnectHost(QString host,quint32 port);
	void DisconnectHost();
	void connectSignal();
	void sendData(QString);
private:
	QTcpSocket *m_sock;
};

#endif // CLIENT_H
