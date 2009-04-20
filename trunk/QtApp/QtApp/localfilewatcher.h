#ifndef LOCALFILEWATCHER_H
#define LOCALFILEWATCHER_H

#include "common.h"
#include "synconf.h"
#include "syncdb.h"
class LocalFileWatcher : public QObject,public CDirectoryChangeHandler
{
	Q_OBJECT

public:
	LocalFileWatcher(QObject *parent);
	~LocalFileWatcher();

private:
	Synconf *synconf;
	SyncDB *syncdb;
	//�����ļ�ͬ��Ŀ¼
	QString strSyncDirectory;
	QPointer<QTimer> timer;
	QMap<QString,PtrFile> ptrfiles;
	
signals:
	void filesChanged(QList<QString> strFiles);
private slots:
	void heartbeat();
};

#endif // LOCALFILEWATCHER_H

