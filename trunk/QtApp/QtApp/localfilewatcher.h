#ifndef LOCALFILEWATCHER_H
#define LOCALFILEWATCHER_H

#include "common.h"
#include "synconf.h"
#include "syncdb.h"
class LocalFileWatcher : public QObject
{
	Q_OBJECT

public:
	LocalFileWatcher(QObject *parent);
	~LocalFileWatcher();

private:
	Synconf *synconf;
	SyncDB *syncdb;
	//本地文件同步目录
	QString strSyncDirectory;
	QPointer<QTimer> timer;
	QMap<QString,PtrFile> ptrfiles;
	
signals:
	void filesChanged(QStringList &strFiles);
private slots:
	void heartbeat();
};

#endif // LOCALFILEWATCHER_H

