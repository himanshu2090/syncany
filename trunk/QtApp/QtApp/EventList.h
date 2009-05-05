#pragma once

#include "syncdb.h"
#include "synconf.h"
class EventList
{
private:
	EventList(void);
public:
	~EventList(void);

	static EventList *instance();
	static void createTable();

	quint32 insertEvent(PtrFile ptrfile,int syncdir);
	quint32 removeEvent(QString strUrl,int syncdir);
private:
	static EventList *m_instance;
	static QString strTableName;
	SyncDB *syncdb;
	Synconf *synconf;
};
