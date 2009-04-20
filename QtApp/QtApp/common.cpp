#include "common.h"


//将map转换为命令行
QString convert_to_cmdline(CommandMap props)
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
		if(keys[i].length()==0 || props[keys[i]].length()==0) continue;
		strCmdLine+=keys[i];
		strCmdLine+="=";
		strCmdLine+=props[keys[i]];
		strCmdLine+=" ";
	}
	return strCmdLine;
}
//从命令行转换为map
CommandMap convert_from_cmdline(QString strCmdLine)
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
			if(kvList[0].length()>0)
			{
				props[QString::number(id++)]=kvList[0];
			}
			break;
		case 2:
			if(kvList[0].length()>0 && kvList[1].length()>0)
			{
				props[kvList[0]]=kvList[1];
			}
			break;
		default://其他情况丢弃
			break;
		}
	}
	return props;
}


