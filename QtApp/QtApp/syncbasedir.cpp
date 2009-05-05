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

//获取内容
QByteArray SyncBaseDir::getData()
{
	return QByteArray();
}

QDateTime SyncBaseDir::getLastModifyTime() //最后修改时间
{
	return modify_time;
}

QDateTime SyncBaseDir::getAnchorTime()
{
	return QDateTime();
}

quint32 SyncBaseDir::getAnchor() //锚点信息
{
	return 0;
}

quint32 SyncBaseDir::getSize() //内容大小
{
	return 0;
}

QString SyncBaseDir::getUrl()//获取路径信息
{
	return strUrl;
}

QString SyncBaseDir::getLocalUrl()//获取localfile信息
{
	return strLocalUrl;
}

quint32 SyncBaseDir::getType()//获取类型（文件，目录或者。。。？）
{
	return TYPE_FOLDER;
}

quint32 SyncBaseDir::setData(QByteArray)//设置内容
{
	return ERR_NOT_SUPPORT;
}

quint32 SyncBaseDir::setLastModifyTime(QDateTime dt) //最后修改时间
{
	modify_time=dt;
	return ERR_NONE;
}

quint32 SyncBaseDir::setAnchor(quint32)//设置锚点
{
	return ERR_NOT_SUPPORT;
}

quint32 SyncBaseDir::setAnchorTime(QDateTime dt)//设置锚点对应的时间信息
{
	return ERR_NOT_SUPPORT;
}

quint32 SyncBaseDir::setUrl(QString url)//设置URL信息
{
	strUrl=url;
	return ERR_NONE;
}

quint32 SyncBaseDir::setLocalUrl(QString url)//设置localfile信息
{
	strLocalUrl=url;
	return ERR_NONE;
}


quint32 SyncBaseDir::setSize(quint32)//设置大小值
{
	return ERR_NOT_SUPPORT;
}

quint32 SyncBaseDir::flush()//将变更记录下来
{
	return ERR_NOT_SUPPORT;
}

