#include "localfilewatcher.h"

LocalFileWatcher::LocalFileWatcher(QObject *parent)
	: QObject(parent)
{
	synconf=Synconf::instance();
	syncdb=SyncDB::instance();
	strSyncDirectory=synconf->getstr("sync_dir","C:/download/");
	timer=new QTimer(this);
	connect(timer.data(), SIGNAL(timeout()), this, SLOT(heartbeat()));
	timer->start(1000);
	ptrfiles=syncdb->sync_files_load_all();
}

LocalFileWatcher::~LocalFileWatcher()
{

}

void list_files(QFileInfo fi,QStringList &dirs)
{
	qDebug("try list_files(%s)",fi.absoluteFilePath().toStdString().c_str());
    QFileInfoList fil=QDir(fi.absoluteFilePath()).entryInfoList();
    for(unsigned int i=0;i<fil.size();++i)
    {
		if(fil[i].fileName()== "." || fil[i].fileName()== "..") //ееЁЩ"." ".."
		{
			qDebug("skip %s",fil[i].absoluteFilePath().toStdString().c_str());
			continue;
		}
		if(fil[i].isFile())
			dirs.append(fil[i].absoluteFilePath());
		else
			list_files(fil[i],dirs);
    }
}

void LocalFileWatcher::heartbeat()
{
    QStringList dirs_all;
	QStringList dirs_update;
    list_files(QFileInfo(strSyncDirectory),dirs_all);
	ptrfiles=syncdb->sync_files_load_all();
	for(quint32 i=0;i<dirs_all.size();++i)
	{
		QMap<QString,PtrFile>::iterator it=ptrfiles.find(dirs_all[i]);
		if(it==ptrfiles.end())
		{
			dirs_update.append(dirs_all[i]);
		}
		else
		{
			QFileInfo qfi(dirs_all[i]);
			PtrFile pf=it.value();
			if(pf->getLastModifyTime()!=qfi.lastModified())
			{
				dirs_update.append(dirs_all[i]);
			}
		}
	}
	emit filesChanged(dirs_update);
}

