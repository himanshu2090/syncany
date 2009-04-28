#ifndef SYNCBASEFILE_H
#define SYNCBASEFILE_H

#include "common.h"
#include "syncdb.h"

class SyncBaseFile : public IFile
{
	Q_OBJECT

public:
	SyncBaseFile(QObject *parent=null);
	~SyncBaseFile();
public:
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

public :
	static void createTable();
	static PtrFile getFileByUrl(QString strUrl);
	static PtrFile getFileByFileName(QString strFilename);
	static QMap<QString,PtrFile> getAllFiles(); //��ȡ���ݿ����ȫ������
private:
	SyncDB *syncdb;
	const char* pszTableName;

	quint32 anchor;
	QString url;
	QString filename;
	quint32 datasize;
	QDateTime lastmodify;
	QDateTime anchortime;
};
#endif // SYNCBASEFILE_H


