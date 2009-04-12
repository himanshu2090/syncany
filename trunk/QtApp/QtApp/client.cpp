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
	m_sock->connectToHost(host,port);
}

void Client::DisconnectHost()
{
	if(m_sock->state()!=QTcpSocket::UnconnectedState)
		m_sock->disconnectFromHost();
} 


void Client::connected ()
{
	emit connected(this);
	emit sigLogger("connected!");
}

void Client::disconnected ()
{
	emit disconnected(this);
	emit sigLogger("disconnected!");
}

void Client::error ( QTcpSocket::SocketError socketError )
{
	emit sigLogger(QString("error:[%1][%2]").arg(socketError).arg(m_sock->errorString()));
}

void Client::hostFound ()
{
	emit sigLogger("hostFound!");
}

void Client::stateChanged ( QTcpSocket::SocketState socketState )
{
	emit sigLogger(QString("stateChanged!%1").arg(socketState));
}

void Client::readData()
{
	buffer+=m_sock->readAll();
	if(bWaitingCommand)
	{
		while(true)
		{
			if(buffer.size()<1)
				return ;
			int lb=buffer.indexOf('\n');
			if(lb==-1)	return ; //�����Ȼ����\n����
			strCmdLine==buffer.left(lb);
			buffer.remove(0,lb+1);
			QStringList strlist=strCmdLine.split(' ');
			if(strlist.size()<2)
			{
				//������������
				emit sigLogger(QString("Error Command Recv(argument is missed!):%1!").arg(strCmdLine));
				continue;
			}
			bool ok;
			strCmdID=strlist[1];
			int nCmdID=strCmdID.toInt(&ok);
			if(!ok)
			{	
				//������������
				emit sigLogger(QString("Error Command Recv( CmdID is not number!):%1!").arg(strCmdLine));
				continue;
			}
			strCmdStr=strlist[0];
			nCmdType=get_cmdtype(strCmdStr.toStdString().c_str());
			if(nCmdType==CMD_UNKNOWN)
			{
				//������������
				emit sigLogger(QString("Error Command Recv( CmdID is not number!):%1!").arg(strCmdLine));
				continue;
			}
			//��ȡ����Я��������
			for(int i=2;i<strlist.size();++i)
			{
				QStringList listKeyValue=strlist[i].split('=');
				if(listKeyValue.size()!=2)
					continue;
				props[listKeyValue[0]]=listKeyValue[1];
			}
			//�ж���û��size���ԣ��������ȴ�����ģʽ������������źţ�������صĲ�ȥ���д���
			if(props.find("size") != props.end())
			{
				int datalen=props["size"].toInt(&ok);
				if(!ok)
				{
					emit sigRecv(this,strCmdID,strCmdStr,props,QByteArray());
					continue;
				}
				bWaitCommand=false;//����ȴ�����ģʽ
				emit sigIn(strCmdLine);
				break;
			}
			//���������ź�
			emit sigRecv(this,strCmdID,strCmdStr,props,QByteArray());
		}
	}
	
	//����Ҫ���ǻ���̫�����Ƿ��¼���ļ���
	
	
	emit sigRecv(this,QString strCmdID,QString strCmdStr,QString QByteArray);
}

void Client::sendData(QString str)
{
	if(m_sock && m_sock->state()==QTcpSocket::ConnectedState)
	{
		QMutexLocker locker(&m_locker_out);
		m_sock->write(str.toAscii());
		emit sigOut(str);
	}
	else
		emit sigLogger(QString("SEND:%s fail! server is not connected!").arg(str));
}


void Client::say_ping(QString strCmdID)
{
	if(m_sock && m_sock->state()==QTcpSocket::ConnectedState)
	{
		QString strCmdStr("ping ");
		strCmdStr+=strCmdID;
		strCmdStr+="\n";
		sendData(strCmdStr);
	}
}

void Client::say_hello(QString strCmdID,QMap<QString,QString> props)
{
	if(m_sock && m_sock->state()==QTcpSocket::ConnectedState)
	{
		QString strCmdStr("hello ");
		strCmdStr+=strCmdID;

		QList<QString> keys=props.keys();

		for(int i=0;i<keys.size();++i)
		{
			strCmdStr+=" ";
			strCmdStr+=keys[i];
			strCmdStr+="=";
			strCmdStr+=props[keys[i]];
		}

		strCmdStr+="\n";
		sendData(strCmdStr);
	}
}