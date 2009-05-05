/**	\file		NTServiceUtils.h
	\brief		C/C++版本的NT Service框架的公用函数

	C/C++版本的NT Service框架的公用函数。负责服务相关的工具函数，包括安装、删除、启动、停止服务，得到服务的
	状态和服务的配置信息等工具函数。
	\version	1.0		Initial release
	\author		葛兵
	\date		2005-12-12
*/
#include <windows.h>
#include <time.h>

////////////////////////////////////////////////////////////////////////////////////////
// Service related routines
////////////////////////////////////////////////////////////////////////////////////////
// Get the service current status. Return NULL for error. User must free the return pointer use free()
static QUERY_SERVICE_CONFIG *GetServiceInfo(LPCSTR lpszServiceName)
{
	SC_HANDLE hSCM;
	SC_HANDLE hService;
	QUERY_SERVICE_CONFIG *rt = NULL;
	DWORD dwBytesNeeded = 0;

	// Open the Service Control Manager
	hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!hSCM) return NULL;

	// Try to open the service
	hService = OpenService(hSCM, lpszServiceName, SERVICE_QUERY_CONFIG);
	if (!hService) { CloseServiceHandle(hSCM); return NULL; }
	// Query service config
	QueryServiceConfig(hService, (QUERY_SERVICE_CONFIG *) NULL, 0, &dwBytesNeeded);
	if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) 
	{
		rt = (QUERY_SERVICE_CONFIG *) malloc(dwBytesNeeded);
		if (!QueryServiceConfig(hService, rt, dwBytesNeeded, &dwBytesNeeded)) { free(rt); rt = NULL; }
	}

	CloseServiceHandle(hService);
	CloseServiceHandle(hSCM);
	return rt;
}

// Get the service current status. Return 0 for service not install, -1 for error
static DWORD GetServiceStatus(LPCSTR lpszServiceName)
{
	SC_HANDLE hSCM;
	SC_HANDLE hService;
	SERVICE_STATUS status;
	int rt;
	// Open the Service Control Manager
	hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!hSCM) return -1;

	// Try to open the service
	hService = OpenService(hSCM, lpszServiceName, SERVICE_QUERY_STATUS);
	if (!hService)
	{
		rt = (GetLastError() == ERROR_SERVICE_DOES_NOT_EXIST) ? 0 : -1;
		CloseServiceHandle(hSCM);
		return rt;
	}
	// Query service state
	QueryServiceStatus(hService, &status);
	CloseServiceHandle(hService);
	CloseServiceHandle(hSCM);
	return status.dwCurrentState;
}

// Install the service. Return 1 for success, 0 for error
static BOOL InstallService(LPCSTR  lpszServiceName, 
						   LPCSTR  lpszDisplayName, 
						   DWORD   dwServiceType, 
						   DWORD   dwStartType, 
						   LPCSTR  lpszCommandLine,
						   LPCSTR  lpszLoadGroup, 
						   LPCSTR  lpszDependencies, 
						   LPCSTR  lpszAccountName, 
						   LPCSTR  lpszAccountPass)
{
	SC_HANDLE hSCM;
	SC_HANDLE hService;
	BOOL bResult = FALSE;
	// Open the Service Control Manager
	hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!hSCM) return FALSE;

	// Create the service
	hService = CreateService(hSCM, 
							 lpszServiceName,		// service name
							 lpszDisplayName,		// display name
							 SERVICE_ALL_ACCESS,	// desired access 
							 dwServiceType,			// service type
							 dwStartType,  			// start type
							 SERVICE_ERROR_NORMAL,	// error control
							 lpszCommandLine,		// service binary path
							 lpszLoadGroup,			// load ordering group
							 NULL,					// tag id
							 lpszDependencies,		// dependencies
							 lpszAccountName,		// account name
							 lpszAccountPass);		// account password
	if (hService)
	{
		bResult = TRUE;
		CloseServiceHandle(hService);
	}

	CloseServiceHandle(hSCM);
	return bResult;
}

// Uninstall the service. Return 1 for success, 0 for error
static BOOL UninstallService(LPCSTR lpszServiceName)
{
	SC_HANDLE hSCM;
	SC_HANDLE hService;
	BOOL bResult = FALSE;
	// Open the Service Control Manager
	hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!hSCM) return FALSE;

	hService = OpenService(hSCM, lpszServiceName, DELETE);
	if (hService)
	{
		if (DeleteService(hService)) bResult = TRUE;
		CloseServiceHandle(hService);
	}

	CloseServiceHandle(hSCM);
	return bResult;
}

// Start the service. Return 1 for success, 0 for error
static BOOL StartExecuteService(LPCSTR lpszServiceName, int dwTimeout)
{
	SC_HANDLE hSCM;
	SC_HANDLE hService;
	SERVICE_STATUS status;
	time_t dwStartTime;
	BOOL bResult = FALSE;
	// Open the Service Control Manager
	hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!hSCM) return FALSE;

	hService = OpenService(hSCM, lpszServiceName, SERVICE_START | SERVICE_QUERY_STATUS);
	if (hService)
	{
		if (StartService(hService, 0, NULL))
		{
			dwStartTime = time(NULL);
			while (QueryServiceStatus(hService, &status))
			{
				if (status.dwCurrentState != SERVICE_START_PENDING) 
				{
					if (status.dwCurrentState == SERVICE_RUNNING) bResult = TRUE;
					else SetLastError(ERROR_GEN_FAILURE);
					break;
				}
				if (time(NULL) - dwStartTime > dwTimeout) 
				{
					SetLastError(ERROR_TIMEOUT);
					break;
				}
				Sleep(500);
			}
		}
		CloseServiceHandle(hService);
	}

	CloseServiceHandle(hSCM);
	return bResult;
}

// Stop the service. Return 1 for success, 0 for error
static BOOL StopExecuteService(LPCSTR lpszServiceName, int dwTimeout)
{
	SC_HANDLE hSCM;
	SC_HANDLE hService;
	SERVICE_STATUS status;
	time_t dwStartTime;
	BOOL bResult = FALSE;
	// Open the Service Control Manager
	hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!hSCM) return FALSE;

	hService = OpenService(hSCM, lpszServiceName, SERVICE_STOP | SERVICE_QUERY_STATUS);
	if (hService)
	{
		if (ControlService(hService, SERVICE_CONTROL_STOP, &status))
		{
			dwStartTime = time(NULL);
			while (QueryServiceStatus(hService, &status))
			{
				if (status.dwCurrentState != SERVICE_STOP_PENDING) 
				{
					if (status.dwCurrentState == SERVICE_STOPPED) bResult = TRUE;
					else SetLastError(ERROR_GEN_FAILURE);
					break;
				}
				if (time(NULL) - dwStartTime > dwTimeout) 
				{
					SetLastError(ERROR_TIMEOUT);
					break;
				}
				Sleep(500);
			}
		}
		CloseServiceHandle(hService);
	}

	CloseServiceHandle(hSCM);
	return bResult;
}

