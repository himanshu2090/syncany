#ifndef CLIENT_H
#define CLIENT_H

#include "common.h"

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
	void error ( QAbstractSocket::SocketError socketError );
	void hostFound ();
	void stateChanged ( QAbstractSocket::SocketState socketState );
//自定义一个信号，用于输出日志
signals:
	void sigOut(QString str);  //自定义信号.输出发送的数据日志
	void sigIn(QString str);  //自定义信号.输出接收的数据日志
	void sigLogger(QString str);  //自定义信号.输出其他辅助日志信息
	void connected(Client *); //表示自己连接成功
	void disconnected(Client *);//表示自己断开
	void sigRecv(Client *,QString strCmdID,QString strCmdStr,QMap<QString,QString> ,QByteArray strCmdData); //表示已经收到一个完整的命令
public:
	QTcpSocket *getSocket(){return m_sock;}
	void ConnectHost(QString host,quint32 port);
	void DisconnectHost();
	void connectSignal();
	void sendData(QString);
	void say_ping(QString strCmdID);
	void say_hello(QString strCmdID,QMap<QString,QString> props);

private:
	QTcpSocket *m_sock;
	QMutex m_locker_out;//向外发送可能是多线程并发，必须同步
	
	bool bWaitingCommand;
	QByteArray buffer; //接收缓冲
	QMap<QString,QString> props;
	int nCmdType;
	QString strCmdID;
	QString strCmdStr;
	QString strCmdLine;
	int datalen;
};

#endif // CLIENT_H
