#ifndef SYNCBASEFILE_H
#define SYNCBASEFILE_H

#include "common.h"

class SyncBaseFile : public IFile
{
	Q_OBJECT

public:
	SyncBaseFile(QObject *parent);
	~SyncBaseFile();
public:
	virtual QDateTime getLastModifyTime(); //����޸�ʱ��
	virtual QDateTime getAnchorTime();
	virtual quint32 getAnchor(); //ê����Ϣ
	virtual quint32 setAnchor(quint32);//����ê��
	virtual quint32 size(); //���ݴ�С
	virtual QByteArray getData();//��ȡ����
	virtual quint32 setData(QByteArray);//��������
	virtual QString getUri();//��ȡ·����Ϣ

private:
	
};

#endif // SYNCBASEFILE_H
