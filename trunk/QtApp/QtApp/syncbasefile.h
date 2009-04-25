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
	virtual QString getUri();//��ȡ·����Ϣ
	virtual QString getLocalUri();//��ȡlocalfile��Ϣ

	virtual quint32 setData(QByteArray);//��������
	virtual quint32 setLastModifyTime(QDateTime); //����޸�ʱ��
	virtual quint32 setAnchor(quint32);//����ê��
	virtual quint32 setAnchorTime(QDateTime dt);//����ê���Ӧ��ʱ����Ϣ
	virtual quint32 setUri(QString strUri);//����URI��Ϣ
	virtual quint32 setLocalUri(QString strUri);//����localfile��Ϣ
	virtual quint32 setSize(quint32);//���ô�Сֵ

	virtual quint32 flush();//�������¼����

public :
	static void createTable();
	static PtrFile getFileByUri(QString strUri);
	static PtrFile getFileByFileName(QString strFilename);
	static QMap<QString,PtrFile> getAllFiles(); //��ȡ���ݿ����ȫ������
private:
	SyncDB *syncdb;
	const char* pszTableName;

	quint32 anchor;
	QString uri;
	QString filename;
	quint32 datasize;
	QDateTime lastmodify;
	QDateTime anchortime;
};
#endif // SYNCBASEFILE_H


