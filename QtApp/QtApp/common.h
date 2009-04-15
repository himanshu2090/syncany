#ifndef _INCLUDE_QT_APP_COMMON_H_
#define _INCLUDE_QT_APP_COMMON_H_

#include <QtCore/QtCore>
#include <QtNetwork/QtNetwork>
#include <QSysInfo>

#include "CppSQLite3.h"

#ifndef null
#define null 0
#endif

#ifndef NULL
#define NULL null
#endif


//�������б�
enum ERROR_CODE_LIST
{
	ERR_NONE=0,
	ERR_UNKNOWN,
	ERR_NETWORK_CONNECT_FAIL,
	ERR_NETWORK_SEND_FAIL,
};
/*
code 	 ���� 	ʹ����
200 	 һ���ʾ�ɹ�ִ�� 	 ��������
201  	 ��Ҫʹ������ͨ�� 	 ��������


400 	���֧�� 	 
401 	��Ҫ��Ȩ  	 
403 	��Ȩʧ��  	cd�� get �� put
404 	Ҫ�����Դ������ 	cd �� get
500 	����ִ���쳣  	��������
501 	ͨ�������쳣 	 ��������
502 	������ʱ������  	 ��������
503 	��������ʱ������  	 ��������

600 	��Ҫע���û�  	whoareyou 
601 	ע����Ϣ��ʽ����ȷ  	alert
602 	�û��������� 	alert 
603 	�û����ѱ�ռ��  	alert
604 	���벻���Ϲ���  	alert 

*/
enum STATE_CODE
{
	STA_OK=200,
	STA_CMD_UNKNOWN=400,
	STA_CMD_EXEC_FAIL=500,
	STA_NOT_FOUND=404,
	STA_AUTH_FAIL=403,
	STA_AUTH_NEED=401,
	STA_NO_SERVICE=502,
};

//������񣩵Ĵ���״̬
enum COMMAND_TAG_LIST
{
	TAG_UNSEND=0,
	TAG_SENDING,
	//���漸���������״̬��
	TAG_COMPLETE,
	TAG_ERROR,
	TAG_ABORT,
	TAG_ABANDON
};

//Э�������б�
enum SYNC_COMMAND
{
	CMD_UNKNOWN=0,
	CMD_HELLO,
	CMD_PING,
	CMD_STATE,
	CMD_ALERT,
	CMD_WHOAREYOU,
	CMD_LS,
	CMD_CD,
	CMD_GET,
	CMD_PUT,
	CMD_RM,
	CMD_MV,
	CMD_BYE,
};

struct SYNC_COMMAND_STR
{
	SYNC_COMMAND cmdid;
	const char *cmdstr;
};

static SYNC_COMMAND_STR cmdlist[]=
{
	//��ʹ��Ƶ������
	{CMD_STATE,"state"},
	{CMD_PING,"ping"},
	{CMD_ALERT,"alert"},
	{CMD_LS,"ls"},
	{CMD_CD,"cd"},
	{CMD_GET,"get"},
	{CMD_PUT,"put"},
	{CMD_RM,"rm"},
	{CMD_MV,"mv"},
	{CMD_WHOAREYOU,"whoareyou"},
	{CMD_HELLO,"hello"},
	{CMD_BYE,"bye"},
};

inline int get_cmdtype(const char *strcmd)
{
	for(int i=0;i<sizeof(cmdlist)/sizeof(SYNC_COMMAND_STR);++i)
	{
		SYNC_COMMAND_STR cmd=cmdlist[i];
		if(QString::compare(strcmd,cmd.cmdstr/* ,Qt::CaseInsensitive */)==0)
			return cmd.cmdid;
	}
	return CMD_UNKNOWN;
}

inline const char * get_cmdstr(int nCmdType)
{
	for(int i=0;i<sizeof(cmdlist)/sizeof(SYNC_COMMAND_STR);++i)
	{
		SYNC_COMMAND_STR cmd=cmdlist[i];
		if(cmd.cmdid==nCmdType)
			return cmd.cmdstr;
	}
	return "";

}

typedef QMap<QString,QString> CommandMap;

//��mapת��Ϊ������
static QString convert_to_cmdline(CommandMap props)
{
	int id=0;
	QString strCmdLine;
	while(1)
	{
		QString strCmdID=QString::number(id++);
		if(props.find(strCmdID)==props.end())
			break;
		strCmdLine+=props[strCmdID];
		strCmdLine+=" ";
	}
	if(id<2) return "";

	QList<QString> keys=props.keys();
	bool ok;
	for(int i=0;i<keys.size();++i)
	{
		keys[i].toInt(&ok);
		if(ok) continue;
		strCmdLine+=keys[i];
		strCmdLine+="=";
		strCmdLine+=props[keys[i]];
		strCmdLine+=" ";
	}
	return strCmdLine;
}
//��������ת��Ϊmap
static CommandMap convert_from_cmdline(QString strCmdLine)
{
	CommandMap props;
	QStringList strList=strCmdLine.split(' ');
	int id=0;
	for(int i=0;i<strList.size();++i)
	{
		QStringList kvList=strList[i].split('=');
		switch(kvList.size())
		{
		case 1:
			props[QString::number(id++)]=kvList[0];break;
		case 2:
			props[kvList[0]]=kvList[1];break;
		default://�����������
			break;
		}
	}
	return props;
}

#endif



