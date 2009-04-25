#include "syncbasedir.h"

SyncBaseDir::SyncBaseDir(QObject *parent)
	: IFolder(parent)
{
	synconf=Synconf::instance();
}

SyncBaseDir::~SyncBaseDir()
{

}

QMap<QString,PtrFile> SyncBaseDir::do_ls(QString strUri)
{

	return QMap<QString,PtrFile>();
}

PtrFolder SyncBaseDir::do_cd(QString strUri)
{
	return PtrFolder();
}

PtrFolder SyncBaseDir::do_mkdir(QString strUri,bool hasFilename)
{
	return PtrFolder();
}

quint32 SyncBaseDir::do_rm(QString strUri)
{
	return 0;
}



