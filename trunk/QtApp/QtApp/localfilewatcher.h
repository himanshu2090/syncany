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

public:
	static QString local2uri(QString);

signals:
	void filesChanged(QStringList &strFiles);
	void filesAdded(QStringList &strFiles);
	void filesRemoved(QStringList &strFiles);
private slots:
	void heartbeat();
};

#endif // LOCALFILEWATCHER_H

