#if !defined(__PmModule__h__2006912)
#define __PmModule__h__2006912

struct PM_MODULE_DATA;

/**	\brief	The PmModule class

*/
class PmModule
{
private:
	PM_MODULE_DATA *m_data;

public:
	PmModule();
	virtual ~PmModule();

	PM_VOID load(PM_LPCSTR szModulePath);
	PM_VOID free();
	PM_HANDLE find(PM_LPCSTR szSymbolName);

	PM_BOOL operator!();
	PM_BOOL operator==(PM_VOID *p);

};

#endif
