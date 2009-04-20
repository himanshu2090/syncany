#ifndef _INCLUDE_QT_APP_COMMON_H_
#define _INCLUDE_QT_APP_COMMON_H_

#include <QtCore/QtCore>
#include <QtNetwork/QtNetwork>
#include <QSysInfo>
#include <QDateTime>

#include "CppSQLite3.h"

#ifndef null
#define null 0
#endif

#ifndef NULL
#define NULL null
#endif


//错误码列表
enum ERROR_CODE_LIST
{
	ERR_NONE=0,
	ERR_UNKNOWN,
	ERR_NETWORK_CONNECT_FAIL,
	ERR_NETWORK_SEND_FAIL,
};
/*
code 	 含义 	使用于
200 	 一般表示成功执行 	 所有命令
201  	 需要使用数据通道 	 所有命令


400 	命令不支持 	 
401 	需要鉴权  	 
403 	鉴权失败  	cd、 get 、 put
404 	要求的资源不存在 	cd 、 get
500 	命令执行异常  	所有命令
501 	通道传输异常 	 所有命令
502 	服务暂时不可用  	 所有命令
503 	服务器暂时不可用  	 所有命令

600 	需要注册用户  	whoareyou 
601 	注册信息格式不正确  	alert
602 	用户名不存在 	alert 
603 	用户名已被占用  	alert
604 	密码不符合规则  	alert 

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

//命令（任务）的处理状态
enum COMMAND_TAG_LIST
{
	TAG_UNSEND=0,
	TAG_SENDING,
	//下面几个都是完成状态！
	TAG_COMPLETE,
	TAG_ERROR,
	TAG_ABORT,
	TAG_ABANDON
};

//协议命令列表
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
	//按使用频度排列
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

//将map转换为命令行
QString convert_to_cmdline(CommandMap props);

//从命令行转换为map
CommandMap convert_from_cmdline(QString strCmdLine);

//接口类
class IFile:public QObject
{
public:
	virtual QDateTime getLastModifyTime()=0; //最后修改时间
	virtual quint32 getAnchor()=0; //锚点信息
	virtual quint32 setAnchor(quint32)=0;//设置锚点
	virtual quint32 size()=0; //内容大小
	virtual QByteArray getData()=0;//获取内容
	virtual quint32 setData(QByteArray)=0;//设置内容
	virtual QString getUri()=0;//获取路径信息
};

typedef QPointer<IFile> PtrFile;

class IFolder:public QObject
{
public:
	virtual QVector<PtrFile> do_ls(QString strUri)=0;
	virtual IFolder *do_cd(QString strUri)=0;
	virtual IFolder *do_mkdir(QString strUri,bool hasFilename)=0;
	virtual quint32 do_rm(QString strUri)=0;
};
typedef QPointer<IFolder> PtrFolder;

class IDevice:public QObject
{
public:
	virtual PtrFolder getFolder(QString strUri)=0;
signals:
	void alert(QString strFile);
};


typedef QPointer<IDevice> PtrDevice;

#endif



