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
	QPointer<QFileSystemWatcher> watcher;
signals:
	void filesChanged(QList<QString> strFiles);
public slots:
	void directoryChanged(const QString &strPath);
};

#endif // LOCALFILEWATCHER_H

