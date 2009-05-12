#ifndef LOCALFILEWATCHER_H
#define LOCALFILEWATCHER_H

#include "common.h"
#include "synconf.h"
#include "syncdb.h"
#include "itraymessage.h"
class LocalFileWatcher : public ITrayMessage
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
	static QString local2url(QString);

private slots:
	void heartbeat();
};

#endif // LOCALFILEWATCHER_H

