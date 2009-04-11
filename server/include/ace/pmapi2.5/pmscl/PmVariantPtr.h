#ifndef __VARIANT__PTR__20060815
#define __VARIANT__PTR__20060815

/**	\brief	The PmVariantPtr class

*/
class PmVariantPtr
{
protected:
	char val_buf[50];
	void* m_data_ptr;
	long m_data_type;
public:
	enum {
		NONE_TYPE = -1, INTEGER = 0, STRING	= 1, BINARY	= 2, CHARACTER = 3
	};
	PmVariantPtr();
	PmVariantPtr(PM_ULONG* valPtr);
	PmVariantPtr(PmString* valPtr);
	PmVariantPtr(PmString& valPtr);
	PmVariantPtr(PM_UCHAR* valPtr);
	PmVariantPtr(char* valPtr);
	void set(char* val, int len);
	operator PM_ULONG();
	operator const char*();
	operator const PM_UCHAR*();
	operator PmString&();
	operator PmString*();
	PmVariantPtr& operator=(PM_ULONG* valPtr);
	PmVariantPtr& operator=(PmString* valPtr);
	PmVariantPtr& operator=(PmString& valPtr);
	PmVariantPtr& operator=(PM_UCHAR* valPtr);
	PmVariantPtr& operator=(char* valPtr);
};

#endif
