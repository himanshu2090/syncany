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
signals:
	void explains(); //如果要自定义槽和信号, explains信号是必须的
	void sigLogger(QString str);  //自定义信号.

private:
	QTcpSocket m_sock;
};

#endif // CLIENT_H
