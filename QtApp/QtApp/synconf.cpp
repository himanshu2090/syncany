#include "synconf.h"

Synconf *Synconf::m_instance=0;

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
	QMutexLocker locker(&g_locker);
	if(m_instance==0)
	{
		m_instance = new Synconf(0);
	}
	return m_instance;
}

//Ӧ�ü��������ļ��Ƿ��޸ģ��ڱ��޸ĵ�ʱ����������
void Synconf::load_conf()
{
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
	QString strConfig;
	QMap<QString,QString>::const_iterator it=m_config.constBegin();
	while(it!=m_config.end())
	{
		strConfig+=it.key()+"="+it.value()+"\n";
	}
}

void Synconf::init_default()//����Ĭ��ֵ��δ���õ�������ΪĬ��ֵ
{
	if(m_config.find("work_dir")==m_config.end())
		m_config["work_dir"]=QDir::currentPath();
	
}

QString Synconf::getstr(QString strKey,QString strDefault)//��ȡ����������
{
	if(m_config.find(strKey)==m_config.end())
		return "";
	return m_config[strKey];
}

void Synconf::setstr(QString strKey,QString strValue,bool flush) //����������
{
	m_config[strKey]=strValue;
	if(flush)
		save_conf();
}
