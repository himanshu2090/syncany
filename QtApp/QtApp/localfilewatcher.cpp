#include "localfilewatcher.h"

LocalFileWatcher::LocalFileWatcher(QObject *parent)
	: QObject(parent)
{
	synconf=Synconf::instance();
	syncdb=SyncDB::instance();
	strSyncDirectory=synconf->getstr("sync_dir","C:/download/");
	connect(timer, SIGNAL(timeout()), this, SLOT(heartbeat()));
	timer->start(heartbeat_interval);
	ptrfiles=syncdb->sync_files_load_all();
}

LocalFileWatcher::~LocalFileWatcher()
{

}
