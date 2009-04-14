#ifndef SYNCONF_H
#define SYNCONF_H

#include "common.h"

//����������� ����ʹ��QSetings�����
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

	void load_conf();
	void init_default();//����Ĭ��ֵ��δ���õ�������ΪĬ��ֵ
public:
	void save_conf();
	static Synconf *instance();
	static QString getOsVersionString();
	QString getstr(QString strKey,QString strDefault=""); //��ȡ����������
	void setstr(QString strKey,QString strValue,bool flush=false); //����������

	QString getinfo(QString strKey);//һЩֻ����Ϣ�Ļ�ȡ
};

#endif // SYNCONF_H
