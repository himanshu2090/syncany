#include "syncbasefile.h"

SyncBaseFile::SyncBaseFile(QObject *parent)
	: QObject(parent)
{

}

SyncBaseFile::~SyncBaseFile()
{

}
QDateTime SyncBaseFile::getLastModifyTime() //����޸�ʱ��
{
}

QDateTime SyncBaseFile::getAnchorTime()
{
}
quint32 SyncBaseFile::getAnchor() //ê����Ϣ
{
}
quint32 SyncBaseFile::setAnchor(quint32)//����ê��
{
}
quint32 SyncBaseFile::size() //���ݴ�С
{
}
QByteArray SyncBaseFile::getData()//��ȡ����
{
}
quint32 SyncBaseFile::setData(QByteArray)//��������
{
}
QString SyncBaseFile::getUri()//��ȡ·����Ϣ
{
}
