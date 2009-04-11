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

void Protocol::server_data(QString str)//收到服务器发送过来的数据
{

}

void say_hello(); //第一个连接发送
void say_ping();//发送ping包
