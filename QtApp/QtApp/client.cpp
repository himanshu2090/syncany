#include "client.h"

Client::Client(QObject *parent)
	: QObject(parent)
	,bWaitingCommand(true)
{
	m_sock=new QTcpSocket();
	connectSignal();
}

Client::~Client()
{
	if(m_sock!=0)
	{
		DisconnectHost();
		delete m_sock;
	}
}

void Client::connectSignal()
{
	connect(m_sock,SIGNAL(connected()),this,SLOT(connected()));
	connect(m_sock,SIGNAL(disconnected()),this,SLOT(disconnected()));
	connect(m_sock,SIGNAL(readyRead()), this, SLOT(readData()));
	connect(m_sock,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(error(QAbstractSocket::SocketError)));
	connect(m_sock,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(stateChanged(QAbstractSocket::SocketState)));
}

void Client::ConnectHost(QString host,quint32 port)
{
	DisconnectHost();
	QMutexLocker locker(&m_locker_out);//������������������
	m_sock->connectToHost(host,port);
}

void Client::DisconnectHost()
{
	QMutexLocker locker(&m_locker_out);
	if(m_sock->state()!=QTcpSocket::UnconnectedState)
		m_sock->disconnectFromHost();
} 


void Client::connected ()
{
	emit sigLogger("connected!");
	emit connected(this);
}

void Client::disconnected ()
{
	emit sigLogger("disconnected!");
	emit disconnected(this);
}

void Client::error ( QTcpSocket::SocketError socketError )
{
	emit sigLogger(QObject::tr("error:[%1][%2]").arg(socketError).arg(m_sock->errorString()));
}

void Client::hostFound ()
{
	emit sigLogger("hostFound!");
}

void Client::stateChanged ( QTcpSocket::SocketState socketState )
{
	emit sigLogger(QObject::tr("stateChanged!%1").arg(socketState));
}

void Client::readData()
{
	QMutexLocker locker(&m_locker_in);
	buffer.append(m_sock->readAll());
	if(bWaitingCommand)
	{
		while(true)
		{
			if(buffer.size()<1)
				return ;
			int lb=buffer.indexOf('\n');
			if(lb==-1)	return ; //�����Ȼ����\n����

			strCmdLine = buffer.left(lb);
			strCmdLine.remove('\r');
			//QByteArray bt = buffer.left(lb);
			////strCmdLine.fromAscii(bt.constData());
			//strCmdLine=bt;
			//strCmdLine=bt.data();
			//char * p=bt.data();
			//strCmdLine=p;

			buffer.remove(0,lb+1);
			QStringList strlist=strCmdLine.split(' ');
			if(strlist.size()<2)
			{
				//������������
				emit sigLogger(QObject::tr("Error Command Recv(argument is missed!):%1!").arg(strCmdLine));
				continue;
			}
			bool ok;

			strCmdID=strlist[1];
			int nCmdID=strCmdID.toInt(&ok);

			if(!ok)
			{	
				//������������
				emit sigLogger(QObject::tr("Error Command Recv( CmdID is not number!):%1!").arg(strCmdLine));
				continue;
			}

			strCmdStr=strlist[0];
			nCmdType=get_cmdtype(strCmdStr.toStdString().c_str());

			if(nCmdType==CMD_UNKNOWN)
			{
				//������������
				emit sigLogger(QObject::tr("Error Command Recv( CmdID is not number!):%1!").arg(strCmdLine));
				continue;
			}
			//��ȡ����Я��������
			props=convert_from_cmdline(strCmdLine);
			
			//�ж���û��size���ԣ��������ȴ�����ģʽ������������źţ�������صĲ�ȥ���д���
			datalen=0;
			if(props.find("size") != props.end())
			{

                datalen=props["size"].toInt(&ok);

				if(!ok)
				{
					emit sigIn(strCmdLine);
					emit sigRecv(this,nCmdID,strCmdStr,strCmdLine,props,QByteArray());
					continue;
				}
				bWaitingCommand=false;//����ȴ�����ģʽ
				emit sigIn(strCmdLine);
				break;
			}
			//���������ź�
			emit sigRecv(this,nCmdID,strCmdStr,strCmdLine,props,QByteArray());
			emit sigIn(strCmdLine);
		}
	}
	
	//����Ҫ���ǻ���̫�����Ƿ��¼���ļ���
	if(buffer.size()>=datalen)
	{
		QByteArray data=buffer.left(datalen);
		buffer.remove(0,datalen);

		emit sigRecv(this,strCmdID.toInt(),strCmdStr,strCmdLine,props,data);
		bWaitingCommand=true;
	}
	else
	{
		emit sigLogger(QDateTime::currentDateTime().toString()+":"+QString::number(buffer.length()));
	}
	//else �����ȴ���һ�����ݵ���
}

int Client::sendData(QByteArray str)
{
	if(m_sock && m_sock->state()==QTcpSocket::ConnectedState)
	{
		QMutexLocker locker(&m_locker_out);
		emit sigOut(str);
		if(m_sock->write(str.data())!=-1)
		{
			return ERR_NONE;
		}
		else
		{
			return ERR_NETWORK_SEND_FAIL;
		}
	}
	else
	{
		emit sigLogger(QObject::tr("SEND:%1 fail! server is not connected!").arg(str.data()));
		return ERR_NETWORK_CONNECT_FAIL;
	}
}

int Client::sendData(QString str)
{
	return sendData(str.toAscii());
}

