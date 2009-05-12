#include "localfilewatcher.h"
#include "syncbasefile.h"
#include "syncbasedir.h"

#define USE_SAWATCH
#ifdef USE_SAWATCH
#include "../SaWatch/src/DWLib.h"
#pragma comment(lib,"DWLib.lib")
#endif

LocalFileWatcher::LocalFileWatcher(QObject *parent)
	: ITrayMessage(parent)
{
	synconf=Synconf::instance();
	syncdb=SyncDB::instance();
	strSyncDirectory=synconf->getstr("sync_dir","C:/download/");
	timer=new QTimer(this);
	connect(timer.data(), SIGNAL(timeout()), this, SLOT(heartbeat()));
	int msec=synconf->getstr("watch_interval","60000").toInt();
	timer->start(msec);
	SyncBaseFile::createTable();
	ptrfiles=SyncBaseFile::getAllFiles();
#ifdef USE_SAWATCH
	AddWatchDir((char *)strSyncDirectory.toStdString().c_str(),"",null);
#endif
}

LocalFileWatcher::~LocalFileWatcher()
{
#ifdef USE_SAWATCH
	CleanApp();
#endif
}

void list_files(QFileInfo fi,QStringList &dirs)
{
	//qDebug("try list_files(%s)",fi.absoluteFilePath().toStdString().c_str());
    QFileInfoList fil=QDir(fi.absoluteFilePath()).entryInfoList();//QDir::NoDotAndDotDot);
    for(unsigned int i=0;i<(unsigned int)fil.size();++i)
    {
		if(fil[i].fileName()== "." || fil[i].fileName()== "..") //排除"." ".."
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
	QStringList dirs_update;
	QStringList dirs_add;
	QStringList dirs_remove;
	QStringList dirs_rename_old;
	QStringList dirs_rename_new;
#ifdef USE_SAWATCH
	//获取一个变更记录，注意是取一条少一条
	/*
	typedef struct {
	long ChangeTime;	// utc-time , you must convert it to time_t(local time)  
	char RecordType;	// D=dir F=file 
	char ChangeType;	// (A)dd,(R)emove,(M)odify,(N)ewName 
	char ChangeFile[MAX_PATH];	// dir or filename 
	char NewName[MAX_PATH];		//* if ChangeType=N, it's a new filename or dirname 
	}SaChangeRecord;
*/
	SaChangeRecord myrecd;
	while(true)
	{
		int nCngCount = GetChange(&myrecd); //获取记录，如果返回小于1， 则表示最近没有变更记录
		if(nCngCount<1)
			break;
		QString strFile=myrecd.ChangeFile;
		strFile.remove(strSyncDirectory);
		strFile.replace('\\','/');
		switch(myrecd.ChangeType)
		{
		case 'A':
			dirs_add.append(strFile);
			break;
		case 'M':
			dirs_update.append(strFile);
			break;
		case 'R':
			dirs_remove.append(strFile);
			break;
		case 'N':
			dirs_rename_old.append(strFile);
			dirs_rename_new.append(myrecd.NewName);
			break;
		default:
			{
				QString strLog;
				strLog="不能识别的文件变化通知："+QString(myrecd.ChangeType)+" / "+myrecd.ChangeFile;
				emit trayLog(strLog);
			}
		}
	}

#else
	QStringList dirs_all;
    list_files(QFileInfo(strSyncDirectory),dirs_all);
	ptrfiles=SyncBaseFile::getAllFiles();
        for(quint32 i=0;i<(quint32)dirs_all.size();++i)
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
#endif
	if(dirs_update.size()>0)
		emit filesChanged(dirs_update);
	if(dirs_add.size()>0)
		emit filesAdded(dirs_add);
	if(dirs_remove.size()>0)
		emit filesRemoved(dirs_remove);
	if(dirs_rename_new.size()>0)
		emit filesRenamed(dirs_rename_old,dirs_rename_new);
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
		qDebug("错误的本地目录：%s",strTemp.toStdString().c_str());
	}
	
	//if(!strTemp.startsWith("/home/wujunping/testfold/"))
	//	return "/home/wujunping/testfold/"+strTemp;
	if(!strTemp.startsWith("/"))
		return "/"+strTemp;
	return strTemp;
}

