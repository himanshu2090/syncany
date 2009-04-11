#ifndef SYNCONF_H
#define SYNCONF_H

#include "common.h"

//�����������
class Synconf : public QObject
{
	Q_OBJECT
private:
	Synconf(QObject *parent=0);
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

	QString getinfo(QString strKey);//һЩֻ����Ϣ�Ļ�ȡ
};

#endif // SYNCONF_H