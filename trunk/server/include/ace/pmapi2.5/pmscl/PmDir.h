#if !defined(__PmDir__h__2006910)
#define __PmDir__h__2006910

struct PM_DIR_DATA;
struct PM_FILE_INFO;

/**	\brief	The PmFileInfo class

*/
class PmFileInfo
{
private:
	PM_FILE_INFO *m_pInfo;

	PmString	m_strName;
	friend class PmDir;
public:
	PmFileInfo();
	virtual ~PmFileInfo();

	PM_LPCSTR name();
};

/**	\brief	The PmDir class

*/
class PmDir
{
private:
    PM_DIR_DATA *m_data;
	
public:
	PmDir();
	virtual ~PmDir();

	/*!
	    \brief open directory
	*/
	PM_VOID open(PM_LPCSTR szDirPath);

	/*!
	    \brief close directory
	*/
	PM_VOID close();

	/*!
	    \brief rewind
	*/
	PM_VOID rewind();

	/*!
	    \brief create a new direcotyr
	*/
	PM_VOID make(PM_LPCSTR szDir);

	/*!
	    \brief create a new directory recursively
	*/
	PM_VOID make_recus(PM_LPCSTR szDir);

	PM_VOID remove(PM_LPCSTR szDir);

	PM_BOOL read(PmFileInfo& refFileInfo);

	/*
		\brief 得到文件列表
	*/
	PM_VOID list(PM_LPCSTR szDir, pm_list &flist);
};

#endif
