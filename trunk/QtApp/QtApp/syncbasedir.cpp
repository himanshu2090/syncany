#include "syncbasedir.h"

SyncBaseDir::SyncBaseDir(QObject *parent)
	: IFolder(parent)
{
	synconf=Synconf::instance();
}

SyncBaseDir::~SyncBaseDir()
{

}


quint32 SyncBaseDir::insert(PtrFile subfile)
{
	subfiles[subfile->getUrl()]=subfile;
	return ERR_NONE;
}
quint32 SyncBaseDir::remove(PtrFile subfile)
{
	return subfiles.remove(subfile->getUrl());
}
QList<PtrFile> SyncBaseDir::getSubFiles()
{
	return subfiles.values();
}

//��ȡ����
QByteArray SyncBaseDir::getData()
{
	return QByteArray();
}

QDateTime SyncBaseDir::getLastModifyTime() //����޸�ʱ��
{
	return modify_time;
}

QDateTime SyncBaseDir::getAnchorTime()
{
	return QDateTime();
}

quint32 SyncBaseDir::getAnchor() //ê����Ϣ
{
	return 0;
}

quint32 SyncBaseDir::getSize() //���ݴ�С
{
	return 0;
}

QString SyncBaseDir::getUrl()//��ȡ·����Ϣ
{
	return strUrl;
}

QString SyncBaseDir::getLocalUrl()//��ȡlocalfile��Ϣ
{
	return strLocalUrl;
}

quint32 SyncBaseDir::getType()//��ȡ���ͣ��ļ���Ŀ¼���ߡ���������
{
	return TYPE_FOLDER;
}

quint32 SyncBaseDir::setData(QByteArray)//��������
{
	return ERR_NOT_SUPPORT;
}

quint32 SyncBaseDir::setLastModifyTime(QDateTime dt) //����޸�ʱ��
{
	modify_time=dt;
	return ERR_NONE;
}

quint32 SyncBaseDir::setAnchor(quint32)//����ê��
{
	return ERR_NOT_SUPPORT;
}

quint32 SyncBaseDir::setAnchorTime(QDateTime dt)//����ê���Ӧ��ʱ����Ϣ
{
	return ERR_NOT_SUPPORT;
}

quint32 SyncBaseDir::setUrl(QString url)//����URL��Ϣ
{
	strUrl=url;
	return ERR_NONE;
}

quint32 SyncBaseDir::setLocalUrl(QString url)//����localfile��Ϣ
{
	strLocalUrl=url;
	return ERR_NONE;
}


quint32 SyncBaseDir::setSize(quint32)//���ô�Сֵ
{
	return ERR_NOT_SUPPORT;
}

quint32 SyncBaseDir::flush()//�������¼����
{
	return ERR_NOT_SUPPORT;
}

