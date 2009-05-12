// DWLib.cpp : Defines the entry point for the DLL application.
//

#pragma warning(disable:4996)

#define _WIN32_WINNT 0x500	//so that I can use ReadDirectoryChanges
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h> 
#include "DWLib.h"
#include "DirectoryChanges.h"
#include "DelayedDirectoryChangeHandler.h"
#include <shlwapi.h>
#include <string>

#include "../bin/sqlite3.h"
#pragma comment (lib,"sqlite3.lib")


CWinApp theApp;
fpChangeCB gChangeCB; 
CDirectoryChangeWatcher	*gDirWatcher;
CDirectoryChangeHandler	*gDirChangeHandler;
sqlite3* gSqlDB;  
char* gCurDir;
HANDLE hDbMutex;
HANDLE hExitEvent;

bool gChangeEventGrab;

DWORD dwChangeFilter =  FILE_NOTIFY_CHANGE_FILE_NAME
|FILE_NOTIFY_CHANGE_DIR_NAME    
|FILE_NOTIFY_CHANGE_SIZE
|FILE_NOTIFY_CHANGE_LAST_WRITE  
|FILE_NOTIFY_CHANGE_CREATION    
/*|FILE_NOTIFY_CHANGE_SECURITY */
/*|FILE_NOTIFY_CHANGE_ATTRIBUTES  */
/*|FILE_NOTIFY_CHANGE_LAST_ACCESS */
;

using namespace std;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			gChangeCB = NULL;
			InitApp();
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

void SqlPrePare(const char* strSql,char* newstr) //����������
{
	for(int i=0; i<strlen(strSql); i++)
	{
		char tmp[] = {strSql[i],0};
		if( tmp[0] == '\'')
			strcat(newstr,"''");
		else
			strcat(newstr,tmp);
	}
}

DWLIB_API int RegChangeNotifyCallBack(fpChangeCB fp,bool IsGrab)
{
	if(fp)
	{
		gChangeCB = fp;
		gChangeEventGrab = IsGrab;
		return 1;
	}
	return 0;	
}

void OnDirChanged(char* pszDir, char changeType)
{
	//������������Ŀ¼������Ϊ���ǲ�ͣ���ڸ������ݿ�������ļ�
	if(strstr(pszDir,gCurDir)) return;
	if(changeType == 'M') return;// Ŀ¼�������Բ�����

	if(gChangeCB)
	{
		const char* fname[2] = {pszDir,""};
		gChangeCB(true,changeType,(char**)fname);
		if(gChangeEventGrab) return;
	}
	

	//Ŀ¼�䶯
	char strSql[2048]="";
	char sqlDir[MAX_PATH*2]="";
	SqlPrePare(pszDir,sqlDir);
	
	char* errMsg;
	int rc = 0;
	switch (changeType)
	{
		//case 'M': // Ŀ¼�������Բ�����
	case 'A':
		sprintf(strSql,"insert into dirhistory(changefile,changetype) values('%s','A');"
			,sqlDir);
		
		if(gSqlDB)
		{
			if(SQLITE_OK != sqlite3_exec(gSqlDB,strSql,0,0,&errMsg))
				printf("error:\n%s\n%s\n",strSql,errMsg);
		}
		break;
	case 'R':
		//ɾ�����ݿ������и�Ŀ¼���ļ��ĸ��ģ�û��Ҫ�ϴ�,��������Ҫ����
		sprintf(strSql
		,"delete from filehistory where changefile like '%s%%\\' and changetype<>'N'); \
		insert into dirhistory(changefile,changetype) values('%s','R');"
		,sqlDir
		,sqlDir);
		
		if(gSqlDB)
		{
			WaitForSingleObject( hDbMutex, INFINITE);
			if(SQLITE_OK != sqlite3_exec(gSqlDB,strSql,0,0,&errMsg))
				printf("error:\n%s\n%s\n",strSql,&errMsg);
			ReleaseMutex(hDbMutex);
		}
		
		break;
	default:
		break;
	}
}

void OnDirRenamed(char* pszOldFile, char* pszNewFileName)
{
	if(gChangeCB)
	{
		const char* fname[2] = {pszOldFile,pszNewFileName};
		gChangeCB(true,'N',(char**)fname);
		if(gChangeEventGrab) return;
	}
		//Ŀ¼������changetype='N'
	//****��ǰ�����и�Ŀ¼�µ��ļ����ĸ��µ���Ŀ¼
	//Ҫע�ⱻ���ӵ�Ŀ¼�����˵����****//
	char strSql[2048]="";
	char SqlOldFile[MAX_PATH*2]="";
	char SqlNewFileName[MAX_PATH*2]="";
		
	SqlPrePare(pszOldFile,SqlOldFile);
	SqlPrePare(pszNewFileName,SqlNewFileName);
	PathAddBackslash(SqlOldFile);
	PathAddBackslash(SqlNewFileName);
	
	char* errMsg;
	sprintf(strSql,"; delete from dirhistory where newname='%s' and changetype='N'; \
		   update filehistory set changefile='%s' where changefile='%s' and (changetype='A' or changetype='M'); \
		   insert into filehistory(changefile,changetype,newname) values('%s','N','%s');"
		   ,SqlOldFile
		   ,SqlNewFileName
		   ,SqlOldFile
		   ,SqlOldFile
		   ,SqlNewFileName);
	
	if(gSqlDB)
	{
		WaitForSingleObject( hDbMutex, INFINITE);

		if(SQLITE_OK != sqlite3_exec(gSqlDB,strSql,0,0,&errMsg))
			printf("error:\n%s\n%s\n",strSql,&errMsg);
		
		ReleaseMutex(hDbMutex);
	}
}
void OnFileChanged(char* pszFile, char changeType)
{
	//������������Ŀ¼������Ϊ���ǲ�ͣ���ڸ������ݿ�������ļ�
	if(strstr(pszFile,gCurDir)) return;

	if(gChangeCB)
	{
		const char* fname[2] = {pszFile,""};
		gChangeCB(false,changeType,(char**)fname);
		if(gChangeEventGrab) return;
	}
	//�ļ��䶯
	//ֻ��Ҫ���һ���޸ĵļ�¼,�����ļ�ֻ��һ����¼��
	//�����'A'���ڣ����豣��'M'��¼,ֻ�轫����޸�ʱ����и���
	char strSql[2048]="";
	char strSqledFile[MAX_PATH*2]="";
	SqlPrePare(pszFile,strSqledFile);

	char* errMsg;
	int rc = 0;
	switch (changeType)
	{
	case 'A': 
		//���ǰ���A��Mû��ȡ�ߣ�ֻ�������һ��A����
		sprintf(strSql,"delete from filehistory where changefile='%s' and (changetype='A' or changetype='M'); \
			insert into filehistory(changefile,changetype) values('%s','A');"
			,strSqledFile
			,strSqledFile);

		if(gSqlDB)
		{
			WaitForSingleObject( hDbMutex, INFINITE);
			
			if(SQLITE_OK != sqlite3_exec(gSqlDB,strSql,0,0,&errMsg))
				printf("error:\n%s\n%s\n",strSql,&errMsg);
			
			ReleaseMutex(hDbMutex);
		}
		break;
	case 'M': 
		//���û��ȡ�ߣ�����ҪM��ֻҪ����A��ʱ�伴��
		if(gSqlDB)
		{
			char **dbResult; 
			int nRow,nColumn;
			
			sprintf(strSql
				,"select cid from filehistory where changefile='%s' and changetype='A' limit 2;"
				,strSqledFile);
			WaitForSingleObject( hDbMutex, INFINITE);
			
			if(SQLITE_OK != sqlite3_get_table( gSqlDB, strSql, &dbResult, &nRow, &nColumn, &errMsg ))
			{
				printf("error:\n%s\n%s\n",strSql,&errMsg);
			}
			else
			{
				if(nRow>0)
				{
					sprintf(strSql
					,"delete from filehistory where changefile='%s' and changetype='A'; \
					insert into filehistory(changefile,changetype) values('%s','A');"	
					,strSqledFile
					,strSqledFile);
				}
				else
				{
					sprintf(strSql
					,"delete from filehistory where changefile='%s' and changetype='M';  \
					insert into filehistory(changefile,changetype) values('%s','M');"	
					,strSqledFile
					,strSqledFile);

				}
				if(SQLITE_OK != sqlite3_exec(gSqlDB,strSql,0,0,&errMsg))
				{
					printf("error:\n%s\n%s\n",strSql,&errMsg);
				}
			}
			sqlite3_free_table( dbResult );	
			ReleaseMutex(hDbMutex);
		}
		break;

	case 'R':
		//ɾ������ǰ�����ݿ�ĸ��ļ�����Ϣ,�ļ���ɾ�˾�û��Ҫ�ϴ��˱仯��Ϣ��//N����
		//���������A����û��ȡ�ߣ�����ҪR��¼,ֻ��Ҫɾ��A��M
		if(gSqlDB)
		{
			char **dbResult; 
			int nRow,nColumn;
			
			sprintf(strSql
				,"select cid from filehistory where changefile='%s' and changetype='A' limit 2;"
				,strSqledFile);
			WaitForSingleObject( hDbMutex, INFINITE);
			
			if(SQLITE_OK != sqlite3_get_table( gSqlDB, strSql, &dbResult, &nRow, &nColumn, &errMsg ))
			{
				printf("error:\n%s\n%s\n",strSql,&errMsg);
			}
			else
			{
				if(nRow>0) //��A
				{
					sprintf(strSql
					,"delete from filehistory where changefile='%s' and (changetype='A' or changetype='M');" 
					,strSqledFile);
				}
				else
				{
					sprintf(strSql,"delete from filehistory where changefile='%s' and changetype<>'N'; \
						insert into filehistory(changefile,changetype) values('%s','R');"
						,strSqledFile
						,strSqledFile);
				}
				if(SQLITE_OK != sqlite3_exec(gSqlDB,strSql,0,0,&errMsg))
				{
					printf("error:\n%s\n%s\n",strSql,&errMsg);
				}
			}
			sqlite3_free_table( dbResult );	
			ReleaseMutex(hDbMutex);
		}
		break;
	default:
		break;
	}
}

void OnFileRenamed(char* pszOldFile, char* pszNewFileName)
{
	if(gChangeCB)
	{
		const char* fname[2] = {pszOldFile,pszNewFileName};
		gChangeCB(false,'N',(char**)fname);
		if(gChangeEventGrab) return;
	}
	// �������ݿ�������=oldfile�ģ�ͨͨ���³�newfilename
	
	
	char strSql[2048]="";
	char SqlOldFile[MAX_PATH*2]="";
	char SqlNewFileName[MAX_PATH*2]="";
	SqlPrePare(pszOldFile,SqlOldFile);
	SqlPrePare(pszNewFileName,SqlNewFileName);
	
	char* errMsg;
	sprintf(strSql,"; delete from filehistory where newname='%s' and changetype='N'; \
		   update filehistory set changefile='%s' where changefile='%s' and (changetype='A' or changetype='M'); \
		   insert into filehistory(changefile,changetype,newname) values('%s','N','%s');"
		   ,SqlOldFile
		   ,SqlNewFileName
		   ,SqlOldFile
		   ,SqlOldFile
		   ,SqlNewFileName);
	
	if(gSqlDB)
	{
		WaitForSingleObject( hDbMutex, INFINITE);
		
		if(SQLITE_OK != sqlite3_exec(gSqlDB,strSql,0,0,&errMsg))
			printf("error:\n%s\n%s\n",strSql,&errMsg);
		ReleaseMutex(hDbMutex);
	}
}


DWLIB_API int EnableWatchDir(char* pszPath,char* pszIncludeFilter, char* pszExcludeFilter)
{
	bool bWatchSubDir=true;

	if(gDirWatcher)
	{
		if( gDirWatcher->IsWatchingDirectory( pszPath) )
			return 0;
		
		DWORD dwWatch = 0;
		if( ERROR_SUCCESS != (dwWatch = gDirWatcher->WatchDirectory(pszPath, 
			dwChangeFilter,
			gDirChangeHandler,
			bWatchSubDir,
			pszIncludeFilter,
			pszExcludeFilter)) )
		{
			printf("Failed to start watch: %d\n",  GetLastError() );
			return dwWatch;
		}
	}
	return 0;
}

DWLIB_API int  AddWatchDir(char* pszPath, char* pszIncludeFilter, char* pszExcludeFilter)
{
	int rc = 0;

	if(strlen(pszPath)<3)
		return -1;
	
	//TODO:������ٲ���
	//Ҫע�����Ŀ¼��б��,����like��������������⼴ C:\aaa\txt.txt ��C:\aaab\txt.txt
	//���ǰҪ����Ƿ��Ѿ����ڸ���Ŀ¼��������򲻱ؼ���
	//����Ƿ��Ѿ�������Ŀ¼����������滻��Ŀ¼
	
	
	char strSql[2048]="";
	char sqlPath[MAX_PATH*2]="";
	char* errMsg;
	
	SqlPrePare(pszPath,sqlPath);
	PathAddBackslash(sqlPath);
	
	WaitForSingleObject( hDbMutex, INFINITE);
	sprintf(strSql, "delete from WatchDirs where dir like '%s%%'; \
		insert into watchdirs(dir,includefilter,excludefilter) values('%s','%s','%s');"
		,sqlPath
		,sqlPath
		,pszIncludeFilter
		,pszExcludeFilter);
	
	if(SQLITE_OK != sqlite3_exec(gSqlDB, strSql, 0, 0, &errMsg))
	{
		printf("err:%s\n",errMsg);
		rc = -2;
		goto errh;
	}

errh:
	ReleaseMutex(hDbMutex);
	return rc;
}

DWLIB_API int DisableWatchDir(char* pszPath)
{
	if(gDirWatcher)
	{
		if( gDirWatcher->IsWatchingDirectory( pszPath) )
			gDirWatcher->UnwatchDirectory( pszPath );
	}
	return 0;
}

DWLIB_API int  RemoveWatchDir(char* pszPath)
{
	int rc = 0;
	if(strlen(pszPath)<3)
		return -1;
	
	//�ȴ����ݿ�ɾ���ٴ��б�ɾ��
	char* errMsg;
	char strSql[2048]="";
	SqlPrePare(pszPath,strSql);
	
	WaitForSingleObject( hDbMutex, INFINITE);
	sprintf(strSql,"delete from watchdirs where dir='%s';", strSql);
	if(SQLITE_OK != sqlite3_exec( gSqlDB, strSql,0,0,&errMsg))
	{
		printf("err:%s\n",errMsg);
		rc = -1;
		ReleaseMutex(hDbMutex);
	}
	else
	{
		ReleaseMutex(hDbMutex);  //careful!!! , next sentence maybe deadlock,so must release it first.
		DisableWatchDir(pszPath);
	}
	
	return 0;
}

DWLIB_API int GetWatchingDirList(SaWatchingDir* pdirlist[],int max_size)
{
	if((!pdirlist) || !max_size) return 0;

	int rc = 0;
	//����ȡ���ӵ�Ŀ¼
	char **dbResult; 
	int nRow,nColumn;
	char* errMsg;
	char strSql[2048]="";
	
	int i,idx = 0;
	SaWatchingDir* pItem;

	sprintf(strSql,"select dir,includefilter,excludefilter from watchdirs limit %d;",max_size);
	
	WaitForSingleObject( hDbMutex, INFINITE);

	if(SQLITE_OK != sqlite3_get_table( gSqlDB, strSql, &dbResult , &nRow , &nColumn , &errMsg ))
	{
		rc = -1;
		goto errh;
	}

	pItem = (SaWatchingDir*)pdirlist;
	for( i=nColumn ; i<= nRow  * nColumn ; i+=nColumn )
	{
		strcpy(pItem->Dir , dbResult[i]);
		strcpy(pItem->IncludeFilter , dbResult[i+1]);
		strcpy(pItem->ExcludeFilter , dbResult[i+2]);
		pItem++;
	}

errh:
	sqlite3_free_table(dbResult);
	ReleaseMutex(hDbMutex);

	return nRow;
	
}

DWLIB_API int GetWatchingDirCount()
{
	int rc = 0;
	//����ȡ���ӵ�Ŀ¼
	char **dbResult; 
	int nRow,nColumn;
	char* errMsg;
	char strSql[2048]="";
	
	sprintf(strSql,"select count(*) from watchdirs");
	
	WaitForSingleObject( hDbMutex, INFINITE);
	
	if(SQLITE_OK != sqlite3_get_table( gSqlDB, strSql, &dbResult , &nRow , &nColumn , &errMsg ))
	{
		rc = 0;
		goto errh;
	}

	rc= atoi(dbResult[nColumn]);
	
errh:
	sqlite3_free_table(dbResult);
	ReleaseMutex(hDbMutex);
	
	return rc;
}


long TimeStamp2Time_t(char* pszTimeStamp)
{
	int ttt[6][4]={{0,5,4,1900}
					,{4,4,2,1}
					,{6,3,2,0}
					,{8,2,2,0}
					,{10,1,2,0}
					,{12,0,2,0}};

	time_t t=time(NULL); struct tm* mytm = localtime(&t);
	for(int i=0;i<6;i++)
	{
		char tmp[5]="";
		memcpy(tmp,((char*)pszTimeStamp)+ttt[i][0],ttt[i][2]);
		*(((int*)mytm)+ttt[i][1])= atoi(tmp)-ttt[i][3];
	}

	return mktime(mytm);
}

DWLIB_API int  GetChange(SaChangeRecord* pChangeRecord)
{
	//����һ���ļ���Ŀ¼�����¼
	//�ȷ���Ŀ¼������ٷ����ļ����
	//��������Ӧ�÷ŵ����е���ǰ��
	
	char **dbResult; 
	int nRow,nColumn;
	char* errMsg;
	char strSql[1024]="";

	int curId=0;
	string curTbl="";
	int rc =0;
	
	WaitForSingleObject( hDbMutex, INFINITE);
	
	curTbl="dirhistory"; //ReNamed dir
	sprintf(strSql,"select cid,strftime('%%Y%%m%%d%%H%%M%%S',changetime) as changetime,changefile,changetype,newname from %s \
		where changetype='N' order by cid limit 1;",curTbl.c_str());
	if(SQLITE_OK == sqlite3_get_table( gSqlDB, strSql, &dbResult , &nRow , &nColumn , &errMsg ))
	{
		if(nRow == 1)
		{
			rc=nRow;
			curId = atoi(dbResult[nColumn]);
			pChangeRecord->RecordType = 'D';
			pChangeRecord->ChangeTime = TimeStamp2Time_t(dbResult[nColumn+1]);
			strcpy(pChangeRecord->ChangeFile,dbResult[nColumn+2]);
			pChangeRecord->ChangeType = dbResult[nColumn+3][0];
			strcpy(pChangeRecord->NewName,dbResult[nColumn+4]);
			goto retout; //break here
		}
	}

	curTbl="dirhistory"; //changed dir
	sprintf(strSql,"select cid,strftime('%%Y%%m%%d%%H%%M%%S',changetime) as changetime,changefile,changetype,newname from %s \
	 where changetype<>'N' order by cid limit 1;",curTbl.c_str());
	if(SQLITE_OK == sqlite3_get_table( gSqlDB, strSql, &dbResult , &nRow , &nColumn , &errMsg ))
	{
	 if(nRow == 1)
	 {
		 rc=nRow;
		 curId = atoi(dbResult[nColumn]);
		 pChangeRecord->RecordType = 'D';
		 pChangeRecord->ChangeTime = TimeStamp2Time_t(dbResult[nColumn+1]);
		 strcpy(pChangeRecord->ChangeFile,dbResult[nColumn+2]);
		 pChangeRecord->ChangeType = dbResult[nColumn+3][0];
		 strcpy(pChangeRecord->NewName,dbResult[nColumn+4]);
		 goto retout;//break here
	 }
	}

	curTbl="filehistory"; //ReNamed file
	sprintf(strSql,"select cid,strftime('%%Y%%m%%d%%H%%M%%S',changetime) as changetime,changefile,changetype,newname from %s \
	 where changetype='N' order by cid limit 1;",curTbl.c_str());
	if(SQLITE_OK == sqlite3_get_table( gSqlDB, strSql, &dbResult , &nRow , &nColumn , &errMsg ))
	{
	 if(nRow == 1)
	 {
		 rc=nRow;
		 curId = atoi(dbResult[nColumn]);
		 pChangeRecord->RecordType = 'F';
		 pChangeRecord->ChangeTime = TimeStamp2Time_t(dbResult[nColumn+1]);
		 strcpy(pChangeRecord->ChangeFile,dbResult[nColumn+2]);
		 pChangeRecord->ChangeType = dbResult[nColumn+3][0];
		 strcpy(pChangeRecord->NewName,dbResult[nColumn+4]);
		 goto retout;//break here
	 }
	}

	curTbl="filehistory"; //ReNamed file
	sprintf(strSql,"select cid,strftime('%%Y%%m%%d%%H%%M%%S',changetime) as changetime,changefile,changetype,newname from %s \
	  where changetype<>'N' order by cid limit 1;",curTbl.c_str());
	if(SQLITE_OK == sqlite3_get_table( gSqlDB, strSql, &dbResult , &nRow , &nColumn , &errMsg ))
	{
	  if(nRow == 1)
	  {
		  rc=nRow;
		  curId = atoi(dbResult[nColumn]);
		  pChangeRecord->RecordType = 'F';
		  pChangeRecord->ChangeTime = TimeStamp2Time_t(dbResult[nColumn+1]);
		  strcpy(pChangeRecord->ChangeFile,dbResult[nColumn+2]);
		  pChangeRecord->ChangeType = dbResult[nColumn+3][0];
		  strcpy(pChangeRecord->NewName,dbResult[nColumn+4]);
		  goto retout;//break here
	  }
	}

retout:
	sqlite3_free_table(dbResult); //free it first.

	if(curId>0)
	{
		sprintf(strSql,"delete from %s where cid=%d;",curTbl.c_str(),curId);
		if(SQLITE_OK != sqlite3_exec(gSqlDB,strSql,0,0,&errMsg))
		{
			printf("err:can not delete record,%s",errMsg);
		}
	}

	ReleaseMutex(hDbMutex);

	return rc;
}

DWLIB_API int InitApp()
{

	hDbMutex = CreateMutex(NULL,FALSE,NULL);

	int rc=0;
	char DllPath[MAX_PATH];  
	
	GetModuleFileName(GetModuleHandle(NULL),DllPath,sizeof(DllPath));  
	char *pt = strrchr(DllPath,'\\');
	*pt=0;
	
	gCurDir =new char[MAX_PATH];
	strcpy(gCurDir,DllPath);
	
	char errMsg[1024]="";
	char dbFile[MAX_PATH]="";
	sprintf(dbFile,"%s\\SaWatch.dat",gCurDir);
	
	rc = sqlite3_open(dbFile , &gSqlDB);
	if( SQLITE_OK != rc)
	{
		sprintf(errMsg,"Can't open database: %s\n",dbFile);
		printf(errMsg);
		//SetLastError(rc);
		//throw errMsg;
		return -1;
	}

	gDirWatcher = new CDirectoryChangeWatcher(FALSE);
	gDirChangeHandler = new CDirectoryChangeHandler;
	
	if(!gDirWatcher || !gDirChangeHandler)
		return -999;
		
	return rc;
}

DWLIB_API int CleanApp()
{
	if(gDirWatcher)
	{
		gDirWatcher->UnwatchAllDirectories();
		delete gDirWatcher;
		gDirWatcher=NULL;

	}

	if(gDirChangeHandler)	
	{
		delete gDirChangeHandler;
		gDirChangeHandler = NULL;
	}
	
	if (gSqlDB)
	{
		sqlite3_close(gSqlDB);
		gSqlDB =NULL;
	}

	//ReleaseMutex( hDbMutex );
	CloseHandle( hDbMutex );
	return 0;
}

