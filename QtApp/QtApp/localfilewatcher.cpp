#include "localfilewatcher.h"
#include "syncbasefile.h"
#include "syncbasedir.h"

LocalFileWatcher::LocalFileWatcher(QObject *parent)
	: QObject(parent)
{
	synconf=Synconf::instance();
	syncdb=SyncDB::instance();
	strSyncDirectory=synconf->getstr("sync_dir","C:/download/");
	timer=new QTimer(this);
	connect(timer.data(), SIGNAL(timeout()), this, SLOT(heartbeat()));
	timer->start(1000);
	SyncBaseFile::createTable();
	ptrfiles=SyncBaseFile::getAllFiles();
}

LocalFileWatcher::~LocalFileWatcher()
{

}

void list_files(QFileInfo fi,QStringList &dirs)
{
	qDebug("try list_files(%s)",fi.absoluteFilePath().toStdString().c_str());
    QFileInfoList fil=QDir(fi.absoluteFilePath()).entryInfoList();//QDir::NoDotAndDotDot);
    for(unsigned int i=0;i<fil.size();++i)
    {
		if(fil[i].fileName()== "." || fil[i].fileName()== "..") //排除"." ".."
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
	ptrfiles=SyncBaseFile::getAllFiles();
	for(quint32 i=0;i<dirs_all.size();++i)
	{
		QString strUri=local2uri(dirs_all[i]);
		QMap<QString,PtrFile>::iterator it=ptrfiles.find(strUri);
		QFileInfo qfi(dirs_all[i]);
		if(it==ptrfiles.end())
		{
			dirs_update.append(strUri);
			PtrFile pf=new SyncBaseFile();
			pf->setLastModifyTime(qfi.lastModified());
			pf->setSize(qfi.size());
			pf->setLocalUri(qfi.absoluteFilePath());
			pf->setUri(strUri);
			pf->flush();
		}
		else
		{
			PtrFile pf=it.value();
			//qDebug(pf->getLastModifyTime().toString().toStdString().c_str());
			//qDebug(qfi.lastModified().toString().toStdString().c_str());
			if(pf->getLastModifyTime().toString()!=qfi.lastModified().toString())
			{
				dirs_update.append(strUri);
				pf->setLastModifyTime(qfi.lastModified());
				pf->setSize(qfi.size());
				pf->setLocalUri(qfi.absoluteFilePath());
				pf->flush();
			}
		}
	}
	if(dirs_update.size()>0)
		emit filesChanged(dirs_update);
}


QString LocalFileWatcher::local2uri(QString strLocal)
{
	Synconf *synconf=Synconf::instance();
	QString strSyncDirectory=synconf->getstr("sync_dir","C:/download/");
	QString strTemp=strLocal.replace('\\','/');
	if(strTemp.startsWith(strSyncDirectory,Qt::CaseInsensitive))
	{
		strTemp.remove(0,strSyncDirectory.length());
	}
	else
	{
		qDebug("错误的本地目录：%s",strTemp.toStdString().c_str());
	}
	if(!strTemp.startsWith("/"))
		return "/"+strTemp;
	return strTemp;
}

