#include "syncbasedir.h"

SyncBaseDir::SyncBaseDir(QObject *parent)
	: IFolder(parent)
{

}

SyncBaseDir::~SyncBaseDir()
{

}
QMap<QString,PtrFile> SyncBaseDir::do_ls(QString strUri)
{

}
PtrFolder SyncBaseDir::do_cd(QString strUri)
{

}
PtrFolder SyncBaseDir::do_mkdir(QString strUri,bool hasFilename)
{

}
quint32 SyncBaseDir::do_rm(QString strUri)
{

}

