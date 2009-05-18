#ifndef SYNCONF_H
#define SYNCONF_H

#include "common.h"

//配置项，单体类 考虑使用QSetings来替代
class Synconf : public QObject
{
	Q_OBJECT
private:
	Synconf(QObject *parent=0);
public:
	~Synconf();
private:
	static Synconf *m_instance;	

	CommandMap m_config;

	void init_default();//将有默认值但未设置的项设置为默认值
public:
	void load_conf();
	void save_conf();
	static Synconf *instance();
	static QString getOsVersionString();
	QString getstr(QString strKey,QString strDefault=""); //获取配置项内容
	quint32 getint(QString strKey,int nDefault=0); //获取配置项内容
	void setstr(QString strKey,QString strValue,bool flush=false); //设置配置项
	void setint(QString strKey,quint32 nValue,bool flush=false); //设置配置项

	QString getinfo(QString strKey);//一些只读信息的获取
signals:
	void keyChanged(QString key);
};

#endif // SYNCONF_H
