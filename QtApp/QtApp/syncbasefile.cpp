#include "syncbasefile.h"

SyncBaseFile::SyncBaseFile(QObject *parent)
	: QObject(parent)
{

}

SyncBaseFile::~SyncBaseFile()
{

}
QDateTime SyncBaseFile::getLastModifyTime() //最后修改时间
{
}

QDateTime SyncBaseFile::getAnchorTime()
{
}
quint32 SyncBaseFile::getAnchor() //锚点信息
{
}
quint32 SyncBaseFile::setAnchor(quint32)//设置锚点
{
}
quint32 SyncBaseFile::size() //内容大小
{
}
QByteArray SyncBaseFile::getData()//获取内容
{
}
quint32 SyncBaseFile::setData(QByteArray)//设置内容
{
}
QString SyncBaseFile::getUri()//获取路径信息
{
}
