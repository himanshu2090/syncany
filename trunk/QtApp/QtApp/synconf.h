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

	void init_default();//����Ĭ��ֵ��δ���õ�������ΪĬ��ֵ
public:
	void load_conf();
	void save_conf();
	static Synconf *instance();
	static QString getOsVersionString();
	QString getstr(QString strKey,QString strDefault=""); //��ȡ����������
	quint32 getint(QString strKey,int nDefault=0); //��ȡ����������
	void setstr(QString strKey,QString strValue,bool flush=false); //����������
	void setint(QString strKey,quint32 nValue,bool flush=false); //����������

	QString getinfo(QString strKey);//һЩֻ����Ϣ�Ļ�ȡ
signals:
	void keyChanged(QString key);
};

#endif // SYNCONF_H
