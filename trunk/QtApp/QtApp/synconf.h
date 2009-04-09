#ifndef SYNCONF_H
#define SYNCONF_H

#include <QtCore/QtCore>
//#include <QObject>
//#include <QMutex>
//#include <QMap>

//�����������
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
	void init_default();//����Ĭ��ֵ��δ���õ�������ΪĬ��ֵ
public:
	static Synconf *instance();
	static QString getOsVersionString();
	QString getstr(QString strKey,QString strDefault=""); //��ȡ����������
	void setstr(QString strKey,QString strValue,bool flush=true); //����������
};

#endif // SYNCONF_H
