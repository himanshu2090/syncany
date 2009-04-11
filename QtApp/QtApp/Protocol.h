#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "common.h"

class Protocol:public QObject
{
	Q_OBJECT
public:
	Protocol(SessionManager *parent);
	~Protocol(void);

	QString get_hellocmd(); //��Session�����Ƶ�һ�����ӷ���
public slots:
	void say_ping();//����ping��
	void do_state(QString str);//�����յ���Ӧ���ݵĲۣ�Ӧ�ý���ŵ���Ӧ���У�
	
	void server_data(QString str);//�յ����������͹���������
signals:
	void send(QString str);//��������ݺ�Ҫͨ���źŽ�Ҫ���͵����ݷ��͸�socket�Ĳ�ȥ����


private:
	SessionManager *sm;
};

#endif //PROTOCOL_H

