#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QObject>
#include <QString>

class Protocol:public QObject
{
	Q_OBJECT
public:
	Protocol(void);
	~Protocol(void);

public slots:
	void do_state(QString str);//�����յ���Ӧ���ݵĲۣ�Ӧ�ý���ŵ���Ӧ���У�

signals:
	void send(QString str);//ͨ���źŽ�Ҫ���͵����ݷ��͸�socket�Ĳ�ȥ����

public:
	void say_hello(); //��һ�����ӷ���
	void say_ping();//����ping��

};

#endif //PROTOCOL_H

