#ifndef SYNCBASEDIR_H
#define SYNCBASEDIR_H

#include "common.h"
#include "synconf.h"

class SyncBaseDir : public IFolder
{
	Q_OBJECT

public:
	SyncBaseDir(QObject *parent=null);
	~SyncBaseDir();
	virtual QMap<QString,PtrFile> do_ls(QString strUri);
	virtual PtrFolder do_cd(QString strUri);
	virtual PtrFolder do_mkdir(QString strUri,bool hasFilename);
	virtual quint32 do_rm(QString strUri);

private:
	Synconf *synconf;
};

#endif // SYNCBASEDIR_H
