#ifndef SYNCONF_H
#define SYNCONF_H

#include <QtCore/QtCore>
//#include <QObject>
//#include <QMutex>
//#include <QMap>

//配置项，单体类
static QMutex g_synconf_locker;
class Synconf : public QObject
{
	Q_OBJECT
private:
	Synconf(){}
	Synconf(QObject *parent);
public:
	~Synconf();
private:
	static Synconf *m_instance;	

	QMap<QString,QString> m_config;

	void load_conf();
	void save_conf();
	void init_default();//将有默认值但未设置的项设置为默认值
public:
	static Synconf *instance();
	static QString getOsVersionString();
	QString getstr(QString strKey,QString strDefault=""); //获取配置项内容
	void setstr(QString strKey,QString strValue,bool flush=true); //设置配置项
};

#endif // SYNCONF_H
