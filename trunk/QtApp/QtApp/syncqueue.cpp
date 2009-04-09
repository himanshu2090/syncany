#include "syncqueue.h"

SyncQueue::SyncQueue(QObject *parent,int type)
	: QObject(parent),m_type(type)
{
	
}

SyncQueue::~SyncQueue()
{

}
