#if !defined(__PmPath__h__2006820)
#define __PmPath__h__2006820

/**	\brief	The PmPath class

*/
class PmPath
{
private:
	PM_HANDLE m_hData;
public:
	PmPath();
	PmPath(const PmPath& path);
	PmPath(PM_LPCSTR szPath);
	virtual ~PmPath();

	void release();
	void loadPath( PM_LPCSTR szPath );
	PM_BOOL isAbsolute() const;

	PmPath& operator= ( PM_LPCSTR );
	PmPath& operator= ( const PmPath& );
	PmPath& operator+= ( PM_LPCSTR );
	PmPath& operator+= ( const PmPath& );

	PmString toString() const;
};
inline PmPath operator+( const PmPath& path, const char* pszPath ){
	PmPath tmp(path);
	tmp += pszPath;
	return tmp;
}
inline PmPath operator+( const char* pszPath, const PmPath& path ){
	PmPath tmp(path);
	tmp += pszPath;
	return tmp;
}
inline PmPath operator+( const PmPath& path1, const PmPath& path2 ){
	PmPath tmp(path1);
	tmp += path2;
	return tmp;
}

#endif
