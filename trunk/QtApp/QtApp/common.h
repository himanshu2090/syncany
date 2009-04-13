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
	{CMD_HELLO,"hello"},
	{CMD_PING,"ping"},
	{CMD_STATE,"state"},
	{CMD_ALERT,"alert"},
	{CMD_WHOAREYOU,"whoareyou"},
	{CMD_LS,"ls"},
	{CMD_CD,"cd"},
	{CMD_GET,"get"},
	{CMD_PUT,"put"},
	{CMD_RM,"rm"},
	{CMD_MV,"mv"},
	{CMD_BYE,"bye"},
};

static int get_cmdtype(const char *strcmd)
{
	for(int i=0;i<sizeof(cmdlist)/sizeof(SYNC_COMMAND_STR);++i)
	{
		SYNC_COMMAND_STR cmd=cmdlist[i];
		if(QString::compare(strcmd,cmd.cmdstr/* ,Qt::CaseInsensitive */)==0)
			return cmd.cmdid;
	}
	return CMD_UNKNOWN;
}

static const char * get_cmdstr(int nCmdType)
{
	for(int i=0;i<sizeof(cmdlist)/sizeof(SYNC_COMMAND_STR);++i)
	{
		SYNC_COMMAND_STR cmd=cmdlist[i];
		if(cmd.cmdid==nCmdType)
			return cmd.cmdstr;
	}
	return "";

}

#endif

