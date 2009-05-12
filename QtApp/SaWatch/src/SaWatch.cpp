// SaWatch.cpp : Defines the entry point for the console application.
//

#pragma warning(disable:4786)
#pragma warning(disable:4996)

#include "SaWatch.h"
#include "DWLib.h"
#include <windows.h>
#include "../3part/NTServiceLib/NTService.cpp"
#include "../3part/sqllite3/sqlite3.h"

#include <stdio.h>
#include <string>
#include "../bin/sqlite3.h"

#pragma comment (lib,"DWLib.lib")
#pragma comment (lib,"sqlite3.lib")

/////////////////////////////////////////////////////////////////////////////
// The one and only application object
sqlite3* gSqlDB;
char* gCurDir;
HANDLE hDbMutex;
HANDLE hExitEvent;

//////////////////////////////////////////////////////////////////////////

void PrintChangesOnScreen(bool IsDir, char ChangeType, char* FileName[])
{
	switch(ChangeType)
	{
	case 'A':
		if(IsDir)
			printf("DIR-Added:%s\n",FileName[0]);
		else
			printf("FILE-Added:%s\n",FileName[0]);
		break;
	case 'M':
		if(IsDir)
			printf("DIR-Modified:%s\n",FileName[0]);
		else
			printf("FILE-Modified:%s\n",FileName[0]);
		break;
	case 'R':
		if(IsDir)
			printf("DIR-Removed:%s\n",FileName[0]);
		else
			printf("FILE-Removed:%s\n",FileName[0]);
		break;
	case 'N':
		if(IsDir)
			printf("DIR-Renamed:%s -> %s\n",FileName[0],FileName[1]);
		else
			printf("FILE-Renamed:%s -> %s\n",FileName[0],FileName[1]);
		break;
	default:
		break;
	}
}

// 实现NT Service框架必须实现的函数
class SaWatchNTService : public CNTService
{
public:
	SaWatchNTService() : CNTService("SaWatch"){}

	int RunService(int argc, char* argv[])
	{
		int rc =0;
		printf("Service is starting...\n");
	
		hExitEvent = CreateEvent(NULL,0,0,0);
		
		RegChangeNotifyCallBack(PrintChangesOnScreen,false);

		///程序进入了运行状态;
		do
		{
			//轮询数据库中要监视的目录列表并监视之
			int nDirCount = GetWatchingDirCount();
			SaWatchingDir** pdirs=NULL;
			pdirs = (SaWatchingDir**)calloc(sizeof(SaWatchingDir),nDirCount);
			
			int nItem = GetWatchingDirList(pdirs,nDirCount);
			SaWatchingDir* pItem = ((SaWatchingDir*)pdirs);
			for(int i=0;i<nItem;i++)
			{
				EnableWatchDir(pItem->Dir,pItem->IncludeFilter,pItem->ExcludeFilter);
				pItem++;
			}
			if(pdirs)
				free(pdirs);
			
		}while(WAIT_TIMEOUT == WaitForSingleObject(hExitEvent,10000)); //等待退出事件
		
		CloseHandle( hExitEvent);
		
		CleanApp();

		return rc;
	}
	
	void StopService()
	{	
		printf("Service is stopping...\n");
		SetEvent(hExitEvent); //发信号退出程序
	}
};

int main(int argc, char* argv[])
{
	SaWatchNTService *service = new SaWatchNTService();
	if (service == NULL)
	{
		printf("Error: Service class derived from CNTService doesn't instance, please check your code.\n");
		return ERROR_INVALID_FUNCTION;
	}

	return service->EnterService(argc, argv);
}
