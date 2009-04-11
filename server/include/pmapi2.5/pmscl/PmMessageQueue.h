#ifndef __PM_MESSAGE_QUEUE_H__
#define __PM_MESSAGE_QUEUE_H__

/**	\brief	The PmMqMessage class

*/
class PmMqMessage : public PmObject
{
protected:
	// uniquely identified the data record in a queue
	PM_ULONG m_ulMsgID;

	// used to identify a message type, then we can select data with flaxibility
	PM_ULONG m_ulMsgType;

	// time when message is put in queue
	PM_ULONG m_ulMsgTime;

	// binary message data
	PmString m_bsMsgData;

public:
	PmMqMessage();
	virtual PM_ULONG getMsgId();
	virtual PM_ULONG getMsgType();
	virtual PM_ULONG getMsgTime();
	virtual ByteStr* getMsgData();
	virtual PM_BOOL setMsgId(PM_ULONG ulMsgID);
	virtual PM_BOOL setMsgType(PM_ULONG ulMsgType);
	virtual PM_BOOL setMsgTime(PM_ULONG ulMsgTime);
	virtual PM_BOOL setMsgData(ByteStr* bsMsgData);
	virtual void dump();
};


#endif
