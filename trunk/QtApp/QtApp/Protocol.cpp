#include "Protocol.h"

Protocol::Protocol(SessionManager *parent):sm(parent)
{
}

Protocol::~Protocol(void)
{
}

void Protocol::do_state(QString str)
{

}

void Protocol::server_data(QString str)//�յ����������͹���������
{

}

void say_hello(); //��һ�����ӷ���
void say_ping();//����ping��
