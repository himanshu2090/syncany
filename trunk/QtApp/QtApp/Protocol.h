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
	void do_state(QString str);//处理收到回应数据的槽（应该将其放到响应队列）

signals:
	void send(QString str);//通过信号将要发送的数据发送给socket的槽去发送

public:
	void say_hello(); //第一个连接发送
	void say_ping();//发送ping包

};

#endif //PROTOCOL_H

