#include "synconf.h"

Synconf *Synconf::m_instance=null;
QMutex g_locker_synconf;

Synconf::Synconf(QObject *parent)
	: QObject(parent)
{
}

Synconf::~Synconf()
{
	save_conf();
}

Synconf * Synconf::instance()
{
	QMutexLocker locker(&g_locker_synconf);
	if(m_instance==null)
	{
		m_instance = new Synconf(0);
		locker.unlock();
		m_instance->load_conf();
	}
	return m_instance;
}


//应该监视配置文件是否被修改，在被修改的时候重新载入
void Synconf::load_conf()
{
	QMutexLocker locker(&g_locker_synconf);
	QString strConfigFile=QDir::currentPath()+"/synconf.ini";
	if(QFile::exists(strConfigFile))
	{
		QFile qf(strConfigFile);
		qf.open(QFile::ReadOnly);
		QString strConfig=qf.readAll();
		qf.close();
		QStringList strList=strConfig.split("\n");
		for(int i=0;i<strList.size();++i)
		{
			QString str=strList[i];
			if(str=="") continue;
			QStringList strKeyValue=str.split("=");
			if(strKeyValue.size()!=2) continue;
			m_config[strKeyValue[0]]=strKeyValue[1];
		}
	}
	init_default();
}

void Synconf::save_conf()
{
	QMutexLocker locker(&g_locker_synconf);
	QString strConfig;
	QMap<QString,QString>::const_iterator it=m_config.constBegin();
	while(it!=m_config.end())
	{
		strConfig+=it.key()+"="+it.value()+"\n";
		++it;
	}
	QString strConfigFile=QDir::currentPath()+"/synconf.ini";
	QFile qf(strConfigFile);
	qf.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Text);
	qf.write(strConfig.toAscii());
	qf.close();
}

void Synconf::init_default()//将有默认值但未设置的项设置为默认值
{
	if(m_config.find("work_dir")==m_config.end())
		m_config["work_dir"]=QDir::currentPath()+"/";
	
}

QString Synconf::getstr(QString strKey,QString strDefault)//获取配置项内容
{
	if(m_config.find(strKey)==m_config.end())
		return strDefault;
	return m_config[strKey];
}

void Synconf::setstr(QString strKey,QString strValue,bool flush) //设置配置项
{
	m_config[strKey]=strValue;
	if(flush)
		save_conf();
}

QString Synconf::getOsVersionString()
{
#if defined(Q_WS_WIN) || defined(Q_OS_CYGWIN)
	switch(QSysInfo::windowsVersion())
	{
	case QSysInfo::WV_32s:
		return "Windows 3.1 with Win32s";
	case QSysInfo::WV_95:
		return "Windows 95";
	case QSysInfo::WV_98:
		return "Windows 98";
	case QSysInfo::WV_Me:
		return "Windows Me";
	case QSysInfo::WV_DOS_based:
		return "MS-DOS-based version of Windows";
	case QSysInfo::WV_NT:
		return "Windows NT";
	case QSysInfo::WV_2000:
		return "Windows 2000";
	case QSysInfo::WV_XP:
		return "Windows XP";
	case QSysInfo::WV_VISTA:
		return "Windows VISTA";
	case QSysInfo::WV_NT_based:
		return "NT-based version of Windows";
	case QSysInfo::WV_CE:
		return "Windows CE";
	case QSysInfo::WV_CENET:
		return "Windows CE .NET";
	case QSysInfo::WV_CE_5:
		return "Windows CE 5.x";
	case QSysInfo::WV_CE_6:
		return "Windows CE 6.x";
	case QSysInfo::WV_CE_based:
		return "CE-based version of Windows";
	}
#endif	
	return "Unknown OS";
}



QString  Synconf::getinfo(QString strKey)
{
	if(strKey=="client_version")
	{
		return "0.0.1";
	}
	if(strKey=="protocol_version")
	{
		return "0.0.1";
	}
	if(strKey=="os_version")
	{
		return getOsVersionString().replace(" ","_");
	}
	return "";
}
