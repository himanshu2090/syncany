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
//�Զ���һ���źţ����������־
signals:
	void sigOut(QString str);  //�Զ����ź�.
	void sigIn(QString str);  //�Զ����ź�.
	void sigLogger(QString str);  //�Զ����ź�.
	void connected(Client *); //��ʾ�Լ����ӳɹ�
	void disconnected(Client *);//��ʾ�Լ��Ͽ�
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
	QMutex m_locker_out;//���ⷢ�Ϳ����Ƕ��̲߳���������ͬ��
};

#endif // CLIENT_H