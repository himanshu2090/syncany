
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DWLIB_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DWLIB_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifndef _DWLIB_H_
#define _DWLIB_H_

#ifdef DWLIB_EXPORTS
#define DWLIB_API extern "C" __declspec(dllexport)
#else
#define DWLIB_API extern "C" __declspec(dllimport)
#endif
#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#ifndef MAX_FILTER_SIZE
#define MAX_FILTER_SIZE 256 //attention!!! -1 for zero end string 
#endif

typedef struct {
	char Dir[MAX_PATH];
	char IncludeFilter[MAX_FILTER_SIZE]; /* eg. .doc */
	char ExcludeFilter[MAX_FILTER_SIZE]; /* eg. .svn */
} SaWatchingDir;

typedef struct {
	long ChangeTime;	/* utc-time , you must convert it to time_t(local time)  */
	char RecordType;	/* D=dir F=file */
	char ChangeType;	/* (A)dd,(R)emove,(M)odify,(N)ewName */
	char ChangeFile[MAX_PATH];	/* dir or filename */
	char NewName[MAX_PATH];		/* if ChangeType=N, it's a new filename or dirname */
}SaChangeRecord;

typedef void (*fpChangeCB)(bool IsDir, char ChangeType, char* FileName[]);   

DWLIB_API int RegChangeNotifyCallBack(fpChangeCB,bool IsGrab);
DWLIB_API int RemoveWatchDir(char* pszPath/*in*/);
DWLIB_API int GetWatchingDirCount();
DWLIB_API int GetWatchingDirList(SaWatchingDir* pdirlist[]/*out*/,int max_size);
DWLIB_API int GetChange(SaChangeRecord* pChangeRecord/*out*/); 
DWLIB_API int AddWatchDir(char* pszPath,char* pszIncludeFilter, char* pszExcludeFilter);/*Always include SubDir*/
DWLIB_API int InitApp();
DWLIB_API int CleanApp();
DWLIB_API int EnableWatchDir(char* pszPath,char* pszIncludeFilter, char* pszExcludeFilter);
DWLIB_API int DisableWatchDir(char* pszPath);


#ifdef DWLIB_EXPORTS
void OnDirChanged(char* pszDir, char changeType);
void OnFileChanged(char* pszFile, char changeType);
void OnFileRenamed(char* pszOldFile, char* pszNewFileName);
void OnDirRenamed(char* pszOldFile, char* pszNewFileName);
#endif

#endif
