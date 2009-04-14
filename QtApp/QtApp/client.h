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
	void sigOut(QString str);  //�Զ����ź�.������͵�������־
	void sigIn(QString str);  //�Զ����ź�.������յ�������־
	void sigLogger(QString str);  //�Զ����ź�.�������������־��Ϣ
	void connected(Client *); //��ʾ�Լ����ӳɹ�
	void disconnected(Client *);//��ʾ�Լ��Ͽ�
	void sigRecv(Client *,quint32 nCmdID,QString strCmdStr,QString strCmdLine,CommandMap ,QByteArray strCmdData); //��ʾ�Ѿ��յ�һ������������
public:
	QTcpSocket *getSocket(){return m_sock;}
	void ConnectHost(QString host,quint32 port);
	void DisconnectHost();
	void connectSignal();
	int sendData(QString);
	int sendData(QByteArray);

private:
	QTcpSocket *m_sock;
	QMutex m_locker_out;//���ⷢ�Ϳ����Ƕ��̲߳���������ͬ��
	QMutex m_locker_in;//�������ݵ�ͬ����
	
	bool bWaitingCommand;
	QByteArray buffer; //���ջ���
	CommandMap props;
	int nCmdType;
	QString strCmdID;
	QString strCmdStr;
	QString strCmdLine;
	int datalen;
};

#endif // CLIENT_H
