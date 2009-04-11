/**
 * file :main.cpp
 */

#include "SyncAny.h"
//进程个数
unsigned int g_ProcessCount = 32;
//每个进程处理多少个连接
unsigned int g_ConnectPerProcess = 512;
//
unsigned int g_ThreadPerProcess = 16;
unsigned int g_Port = 18120;
unsigned int g_Timeout = 30 * 60;
unsigned int g_nBacklog  = 4096;
unsigned int g_nAsyncAcceptProcessCount = 4;


void LoadConf(const char* pszModuleName)
{
	g_ProcessCount = 		SyncConfGetInt(pszModuleName,"ProcessCount",4);
	g_ConnectPerProcess =	SyncConfGetInt(pszModuleName,"ConnectPerProcess",512);
	g_ThreadPerProcess =	SyncConfGetInt(pszModuleName,"ThreadPerProcess",16);
	g_Port = 				SyncConfGetInt(pszModuleName,"Port",18120);
	g_Timeout = 			SyncConfGetInt(pszModuleName,"Timeout",1800);
	g_nBacklog = 			SyncConfGetInt(pszModuleName,"backlog",4096);
	
/*
	g_ProcessCount = migLocalConfGetInt("ProcessCount",2);
	g_ConnectPerProcess =  migLocalConfGetInt("ConnectPerProcess",512);
	g_ThreadPerProcess = migLocalConfGetInt("ThreadPerProcess",16);
	g_Port = migLocalConfGetInt("Port",18025);
	g_Timeout = migLocalConfGetInt("Timeout",180);
	g_nBacklog = migLocalConfGetInt("backlog",4096);
*/
}





//main route
int main(int argc,char * argv[]) 
{
	int iret = 0;
	try
	{
		if ( argc > 1 ) 
		{
			//通过 命令行参数的形式将socket描述字传递给子进程。
			SyncFrame frame;
			frame.WorkProcess(argv[1],argv[2],argv[3],argv[4],argv[5],argv[6]);
		}
		else 
		{
			//read cmsmtp configuration info from mig.conf file at $MIG_HOME/conf/mig.conf
			//仅在父进程中读取配置文件
			LoadConf(argv[0]);
			pm_debug5("port<%d>,processcnt<%d>,threadcnd<%d>,connectcnt<%d>,timeout<%d>",g_Port,g_ConnectPerProcess,g_ThreadPerProcess,g_ConnectPerProcess,g_Timeout);
			//PmTime Now = PmTime::getCurrentTime();
			PmString strSemaphoreName = "SYNC_ANY_GLOBLE_SEMAPHORE";
			pm_info1("globle semaphore name<%s>",strSemaphoreName.c_str());
			//ACE_Process_Semaphore  ProcessSemaphore(nAsyncAcceptProcessCount,strSemaphoreName.c_str());
			ACE_SV_Semaphore_Complex  ProcessSemaphore(ACE_TEXT_ALWAYS_CHAR (strSemaphoreName.c_str()),ACE_SV_Semaphore_Complex::ACE_CREATE,g_nAsyncAcceptProcessCount);

			SyncFrame frame;
			frame.DispatchProcess(argv[0],g_Port,g_ProcessCount,g_ThreadPerProcess,g_ConnectPerProcess,g_Timeout,g_nBacklog,strSemaphoreName.c_str(),g_nAsyncAcceptProcessCount);
			ProcessSemaphore.remove();
		}

	}
	catch( PmException& ex )
	{
		pm_error1("fail:%s", ex->getText().c_str());
	}
	return iret;
}



