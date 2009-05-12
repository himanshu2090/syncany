#pragma once

#include "syncdb.h"
#include "synconf.h"
#include "itraymessage.h"

class EventList:public ITrayMessage
{
private:
	EventList(void);
public:
	~EventList(void);

	static EventList *instance();
	static void createTable();

	quint32 insertEvent(PtrFile ptrfile,int syncdir,int syncop);
	quint32 getNextEvent(int &eid,QString &strUrl,int &syncdir,int &syncop,bool remove=false);
	quint32 removeEvent(QString strUrl,int syncdir);
private:
	static EventList *m_instance;
	static QString strTableName;
	SyncDB *syncdb;
	Synconf *synconf;
};
