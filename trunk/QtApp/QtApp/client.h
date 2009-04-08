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
//�Զ���һ���źţ����������־
signals:
	void explains(); //���Ҫ�Զ���ۺ��ź�, explains�ź��Ǳ����
	void sigOut(QString str);  //�Զ����ź�.
	void sigIn(QString str);  //�Զ����ź�.
	void sigLogger(QString str);  //�Զ����ź�.

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
