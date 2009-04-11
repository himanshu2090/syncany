#ifndef __DATABASE__TABLE__COLUMN__20050815
#define __DATABASE__TABLE__COLUMN__20050815

typedef PM_BOOL (*GMQDataEscapeStringFunc)(void *first_arg, ByteStr *data);

/**	\brief	The PmDbColumn class

*/
class PmDbColumn
{
private:
	PM_ULONG m_ulval;
protected:
	PmString		m_strColName;
	PmVariantPtr	m_varColValue;
public:
	PmDbColumn();
	PmDbColumn(const char* pszName, PmVariantPtr& value);
	PM_LPCSTR getName() const;
	PM_LPCSTR getValue();
	PM_BOOL setName(PM_LPCSTR name);
	PM_BOOL setValue(PM_LPCSTR value);
};

#endif

