#ifndef __PM__FILE__
#define __PM__FILE__

/**	\brief	The PmFile class

*/
class PmFile
{
private:
	PmFile &  operator=(class PmFile &);
	PmFile (class PmFile &);

public:
	PmFile();
	virtual ~PmFile();

	PM_VOID close();
	PM_VOID open(PM_LPCSTR lpszcFileName, char chAccessFlag='r',PM_BOOL bOpenAlways=PM_FALSE,int iFileSecureAttr=PM_FSA_NORMAL);
	PM_VOID create( PM_LPCSTR pszcFileName, PM_BOOL bAlwaysNew=PM_TRUE, int iFileSecureAttr=PM_FSA_NORMAL );
	PM_VOID writeString(PM_LPCSTR lpszcData);
	PM_VOID print( PM_LPCSTR pszFormat, ... );
	PM_VOID vprint( PM_LPCSTR pszFormat, PM_ARGLIST args );
	PM_BOOL readLine(PM_LPSTR lpszBuffer,PM_UINT uiMax);

	PM_INT getPosition() const;
	
	PM_VOID readAll( PmString & str );
	PM_INT read(PM_LPVOID lpBuf, PM_UINT uiCount);
	PM_INT write(PM_LPCVOID lpcBuf, PM_UINT uiCount);
	PM_INT seek(PM_LONG lOff, PM_UINT uiFrom);
	PM_INT getFileSize();
	PM_VOID flush();



	PM_LPCSTR getFileName();

	PM_HANDLE getHandle();
	PM_BOOL isEof();

//	DWORD GetPosition() const;
//	BOOL GetStatus(CFileStatus& rStatus) const;
//	CString GetFileName() const;
//	CString GetFileTitle() const;
//	CString GetFilePath() const;


//	PM_BOOL seekToEnd();
//	PM_BOOL seekToBegin();

private:
	PM_HANDLE m_hFile;
	PM_CHAR m_chFileName[PM_MAX_PATH];
};



#endif
