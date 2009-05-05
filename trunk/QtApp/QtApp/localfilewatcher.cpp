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
	timer->start(10000);
	SyncBaseFile::createTable();
	ptrfiles=SyncBaseFile::getAllFiles();
}

LocalFileWatcher::~LocalFileWatcher()
{

}

void list_files(QFileInfo fi,QStringList &dirs)
{
	//qDebug("try list_files(%s)",fi.absoluteFilePath().toStdString().c_str());
    QFileInfoList fil=QDir(fi.absoluteFilePath()).entryInfoList();//QDir::NoDotAndDotDot);
    for(unsigned int i=0;i<fil.size();++i)
    {
		if(fil[i].fileName()== "." || fil[i].fileName()== "..") //�ų�"." ".."
		{
			//qDebug("skip %s",fil[i].absoluteFilePath().toStdString().c_str());
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
	QStringList dirs_add;
	QStringList dirs_remove;
    list_files(QFileInfo(strSyncDirectory),dirs_all);
	ptrfiles=SyncBaseFile::getAllFiles();
	for(quint32 i=0;i<dirs_all.size();++i)
	{
		QString strUrl=local2url(dirs_all[i]);
		QMap<QString,PtrFile>::iterator it=ptrfiles.find(strUrl);
		QFileInfo qfi(dirs_all[i]);
		if(it==ptrfiles.end())
		{
			dirs_add.append(strUrl);
			PtrFile pf=new SyncBaseFile();
			pf->setLastModifyTime(qfi.lastModified());
			pf->setSize(qfi.size());
			pf->setLocalUrl(qfi.absoluteFilePath());
			pf->setUrl(strUrl);
			pf->flush();
		}
		else
		{
			PtrFile pf=it.value();
			ptrfiles.remove(it.key());
			//qDebug(pf->getLastModifyTime().toString().toStdString().c_str());
			//qDebug(qfi.lastModified().toString().toStdString().c_str());
			if(pf->getLastModifyTime().toString()!=qfi.lastModified().toString())
			{
				dirs_update.append(strUrl);
				pf->setLastModifyTime(qfi.lastModified());
				pf->setSize(qfi.size());
				pf->setLocalUrl(qfi.absoluteFilePath());
				pf->flush();
			}
		}
	}
	QList<QString> ks=ptrfiles.keys();
	for(int i=0;i<ks.size();++i)
	{
		PtrFile pf=ptrfiles[ks[i]];
		dirs_remove.append(ks[i]);
	}
	if(dirs_update.size()>0)
		emit filesChanged(dirs_update);
	if(dirs_add.size()>0)
		emit filesAdded(dirs_add);
	if(dirs_remove.size()>0)
		emit filesRemoved(dirs_remove);
}


QString LocalFileWatcher::local2url(QString strLocal)
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
		qDebug("����ı���Ŀ¼��%s",strTemp.toStdString().c_str());
	}
	
	//if(!strTemp.startsWith("/home/wujunping/testfold/"))
	//	return "/home/wujunping/testfold/"+strTemp;
	if(!strTemp.startsWith("/"))
		return "/"+strTemp;
	return strTemp;
}
