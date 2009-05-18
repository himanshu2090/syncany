#ifndef _INCLUDE_QT_APP_COMMON_H_
#define _INCLUDE_QT_APP_COMMON_H_

#include <QtCore/QtCore>
#include <QtGui/QtGui>
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

//常用KEY=VALUE的KEY定义
#define KEY_CMD "0"
#define KEY_CMDID "1"
#define KEY_STATECODE "2"

#define KEY_USER "user"
#define KEY_PASS "pass"
#define KEY_URL "url"
#define KEY_URL_NEW "new_url"
#define KEY_TYPE "type"
#define KEY_REASON "reason"
#define KEY_SERVER_IP "serverip"
#define KEY_SERVER_PORT "serverport"
#define KEY_SESSION_ID "session"
#define KEY_DEVICE_ID "deviceid"
#define KEY_ACTION_TYPE "action_type"
#define KEY_SENDER "sender"
#define KEY_VERSION "version"
#define KEY_SYNCDIR "syncdir"
#define KEY_WORK_DIR "work_dir"

//同步方向
enum SYNC_DIR_LIST
{
	SYNC_TO_SERVER=1,
	SYNC_FROM_SERVER=2,
};
//同步操作列表
enum SYNC_OP_LIST
{
	SYNC_OP_ADD=1,
	SYNC_OP_REMOVE,
	SYNC_OP_MODIFY,
	SYNC_OP_RENAME
};
//常用ALERT 的type
#define ALERT_TYPESTR_SIGNUP	"signup"
#define ALERT_TYPESTR_SINGLE_CHOICE	"singlechoice"
#define ALERT_TYPESTR_MULTI_CHOICE	"mutilchoice"
#define ALERT_TYPESTR_INPUT		"input"
#define ALERT_TYPESTR_REDIRECT	"redirect" 
#define ALERT_TYPESTR_NOTIFY	"notify"
//错误码列表
enum ERROR_CODE_LIST
{
	ERR_NONE=0,
	ERR_UNKNOWN,
	ERR_NODATA,
	ERR_NETWORK_CONNECT_FAIL,
	ERR_NETWORK_SEND_FAIL,
	ERR_NOT_SUPPORT,
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
        for(unsigned int i=0;i<sizeof(cmdlist)/sizeof(SYNC_COMMAND_STR);++i)
	{
		SYNC_COMMAND_STR cmd=cmdlist[i];
		if(QString::compare(strcmd,cmd.cmdstr/* ,Qt::CaseInsensitive */)==0)
			return cmd.cmdid;
	}
	return CMD_UNKNOWN;
}

inline const char * get_cmdstr(int nCmdType)
{
        for(unsigned int i=0;i<sizeof(cmdlist)/sizeof(SYNC_COMMAND_STR);++i)
	{
		SYNC_COMMAND_STR cmd=cmdlist[i];
		if(cmd.cmdid==nCmdType)
			return cmd.cmdstr;
	}
	return "";

}

typedef QMap<QString,QString> StringMap;
typedef StringMap CommandMap;
typedef StringMap AlertMessage;
typedef QList<StringMap> AlertMessageList;

QByteArray raw_url_decode(QByteArray str);
QByteArray raw_url_encode(QByteArray str);

//将map转换为命令行
QString convert_to_cmdline(CommandMap props);

//从命令行转换为map
CommandMap convert_from_cmdline(QString strCmdLine);

//接口类
enum FILE_TYPE_LIST { TYPE_FILE,TYPE_FOLDER};

class IFile:public QObject
{
public:
	IFile(QObject *parent=null):QObject(parent){}
	virtual QByteArray getData()=0;//获取内容
	virtual QDateTime getLastModifyTime()=0; //最后修改时间
	virtual QDateTime getAnchorTime()=0;
	virtual quint32 getAnchor()=0; //锚点信息
	virtual quint32 getSize()=0; //内容大小
	virtual QString getUrl()=0;//获取路径信息
	virtual QString getLocalUrl()=0;//获取localfile信息

	virtual quint32 getType()=0;//获取类型（文件，目录或者。。。？）

	virtual quint32 setData(QByteArray)=0;//设置内容
	virtual quint32 setLastModifyTime(QDateTime)=0; //最后修改时间
	virtual quint32 setAnchor(quint32)=0;//设置锚点
	virtual quint32 setAnchorTime(QDateTime dt)=0;//设置锚点对应的时间信息
	virtual quint32 setUrl(QString strUrl)=0;//设置URL信息
	virtual quint32 setLocalUrl(QString strUrl)=0;//设置localfile信息
	virtual quint32 setSize(quint32)=0;//设置大小值
	virtual quint32 flush()=0;//将变更记录下来
};

typedef QPointer<IFile> PtrFile;

class IFolder;
typedef QPointer<IFolder> PtrFolder;
class IFolder:public IFile
{
public:
	IFolder(QObject *parent=null):IFile(parent){}
	virtual quint32 insert(PtrFile subfile)=0;
	virtual quint32 remove(PtrFile subfile)=0;
	virtual QList<PtrFile> getSubFiles()=0;
};

class IDevice:public QObject
{
public:
	IDevice(QObject *parent=null):QObject(parent){}
	virtual PtrFolder getFolder(QString strUrl)=0;
signals:
	void alert(QString strFile);
};
typedef QPointer<IDevice> PtrDevice;

//根据类型信息串，创建相应的对象
PtrFile createFileObject(QString strType);
PtrFile getFileObjectByUrl(QString strUrl);
#endif



