// SaWatch.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SaWatch.h"
#include "DirectoryChanges.h"
#include "DelayedDirectoryChangeHandler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

CString GetLastErrorMessageString(DWORD dwLastError )
{
	LPVOID lpMsgBuf = NULL;
	FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
		dwLastError, 
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf, 0, NULL );
	
	CString str = (LPCTSTR)lpMsgBuf;
	
	if( lpMsgBuf != NULL )
		LocalFree(lpMsgBuf);	
	
	return str;
}

void startWatch()
{
	CDirectoryChangeWatcher	m_DirWatcher(FALSE);
	CDirectoryChangeHandler	m_DirChangeHandler;

	CString	m_strDirectoryToMonitor = "C:\\test\\"; 


	BOOL bWatchSubDir = TRUE;
	

	CString m_strIncludeFilter1;
	m_strIncludeFilter1.Empty();
	CString	m_strExcludeFilter1;
	m_strExcludeFilter1.Empty();

	
	DWORD dwChangeFilter = FILE_NOTIFY_CHANGE_FILE_NAME 
		| FILE_NOTIFY_CHANGE_DIR_NAME 
		| FILE_NOTIFY_CHANGE_LAST_WRITE 
		| FILE_NOTIFY_CHANGE_LAST_ACCESS 
		| FILE_NOTIFY_CHANGE_SIZE 
		| FILE_NOTIFY_CHANGE_ATTRIBUTES 
		| FILE_NOTIFY_CHANGE_LAST_WRITE 
		| FILE_NOTIFY_CHANGE_LAST_ACCESS 
		| FILE_NOTIFY_CHANGE_CREATION 
		| FILE_NOTIFY_CHANGE_SECURITY;

	if( m_DirWatcher.IsWatchingDirectory( m_strDirectoryToMonitor) )
		m_DirWatcher.UnwatchDirectory( m_strDirectoryToMonitor );
	
	
	DWORD dwWatch = 0;
	if( ERROR_SUCCESS != (dwWatch = m_DirWatcher.WatchDirectory(m_strDirectoryToMonitor, 
		dwChangeFilter,
		&m_DirChangeHandler,
		bWatchSubDir,
		m_strIncludeFilter1,
		m_strExcludeFilter1)) )
	{
		printf("Failed to start watch: %s\n",  GetLastErrorMessageString( dwWatch ) );
	}
	
	while(1)
		Sleep(200);
}



int main(int argc, char* argv[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
		return -1;

	startWatch();

	while(1)
		Sleep(200);

	return nRetCode;
}


