#ifndef SYNCBASEDIR_H
#define SYNCBASEDIR_H

#include "common.h"
#include "synconf.h"

class SyncBaseDir : public IFolder
{
	Q_OBJECT

public:
	SyncBaseDir(QObject *parent=null);
	~SyncBaseDir();

	//File virtual function
	virtual QByteArray getData();//��ȡ����
	virtual QDateTime getLastModifyTime(); //����޸�ʱ��
	virtual QDateTime getAnchorTime();
	virtual quint32 getAnchor(); //ê����Ϣ
	virtual quint32 getSize(); //���ݴ�С
	virtual QString getUrl();//��ȡ·����Ϣ
	virtual QString getLocalUrl();//��ȡlocalfile��Ϣ

	virtual quint32 getType();//��ȡ���ͣ��ļ���Ŀ¼���ߡ���������

	virtual quint32 setData(QByteArray);//��������
	virtual quint32 setLastModifyTime(QDateTime); //����޸�ʱ��
	virtual quint32 setAnchor(quint32);//����ê��
	virtual quint32 setAnchorTime(QDateTime dt);//����ê���Ӧ��ʱ����Ϣ
	virtual quint32 setUrl(QString strUrl);//����URI��Ϣ
	virtual quint32 setLocalUrl(QString strUrl);//����localfile��Ϣ
	virtual quint32 setSize(quint32);//���ô�Сֵ

	virtual quint32 flush();//�������¼����

	//Folder virtual function

	virtual quint32 insert(PtrFile subfile);
	virtual quint32 remove(PtrFile subfile);
	virtual QList<PtrFile> getSubFiles();

private:
	Synconf *synconf;
	QMap<QString ,PtrFile> subfiles;
	QDateTime modify_time;
	QString strUrl;
	QString strLocalUrl;
};

#endif // SYNCBASEDIR_H
