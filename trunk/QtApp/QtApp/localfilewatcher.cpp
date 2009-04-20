#include "localfilewatcher.h"

LocalFileWatcher::LocalFileWatcher(QObject *parent)
	: QObject(parent)
{
	synconf=Synconf::instance();
	syncdb=SyncDB::instance();
	strSyncDirectory=synconf->getstr("sync_dir","C:/download/");
	watcher=new QFileSystemWatcher(this);
	watcher->addPath(strSyncDirectory);

	connect(watcher.data(),SIGNAL(directoryChanged( const QString &)),this,SLOT(directoryChanged(const QString &)));
	//connect(watcher.data(),SIGNAL(fileChanged ( const QString &)),this,SLOT(log(QString)));


}

LocalFileWatcher::~LocalFileWatcher()
{

}

void LocalFileWatcher::directoryChanged(const QString &strPath)
{
	QList<PtrFile> ptrfiles=syncdb->sync_files_load_all();
	QDir dirhelper=QDir(strPath);

	QList<QString> strFiles;
	QFileInfoList fil=dirhelper.entryInfoList();
	for(int i=0;i<fil.size();++i)
	{
		QFileInfo fi=fil[i];
		strFiles.append(fi.absoluteFilePath());
	}

	emit filesChanged(strFiles);
}

