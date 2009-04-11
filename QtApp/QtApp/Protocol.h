#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "common.h"

class Protocol:public QObject
{
	Q_OBJECT
public:
	Protocol(SessionManager *parent);
	~Protocol(void);

	QString get_hellocmd(); //由Session来控制第一个连接发送
public slots:
	void say_ping();//发送ping包
	void do_state(QString str);//处理收到回应数据的槽（应该将其放到响应队列）
	
	void server_data(QString str);//收到服务器发送过来的数据
signals:
	void send(QString str);//处理好数据后，要通过信号将要发送的数据发送给socket的槽去发送


private:
	SessionManager *sm;
};

#endif //PROTOCOL_H

