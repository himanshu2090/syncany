/**	\file		NTServiceCore.h
	\brief		C/C++版本的NT Service框架的公用函数

	C/C++版本的NT Service框架的公用函数，负责框架的处理函数
	\version	1.0		Initial release
	\author		葛兵
	\date		2005-12-12
*/
#include <windows.h>
#include <stdio.h>
#include <time.h>

///	@cond invisible
#define	SERVICE_WAIT_TIMEOUT	10
#define	SAFE_COPY(dst, src)		strncpy(dst, src ? src : "", sizeof(dst)); dst[sizeof(dst)-1] = 0;

////////////////////////////////////////////////////////////////////////////////////////
// global functions
////////////////////////////////////////////////////////////////////////////////////////
void WINAPI base_print_usage_func();

////////////////////////////////////////////////////////////////////////////////////////
// static variables
////////////////////////////////////////////////////////////////////////////////////////
static char  s_szStringBuffer[3000];
static BOOL  s_bIsService = FALSE;
static char  s_szServiceName[300] = {0};
static char  s_szDisplayName[300] = {0};
static char  s_szLoadOrderGroup[300] = {0};
static char  s_szDependencyGroup[300] = {0};
static char  s_szStartUsername[300] = {0};
static char  s_szStartPassword[300] = {0};
static DWORD s_dwServiceType = SERVICE_WIN32_OWN_PROCESS;
static DWORD s_dwStartType = SERVICE_AUTO_START;
static int   s_dwRuntimeArgc = 0;
static char**s_lpRuntimeArgv = NULL;
static SERVICE_STATUS s_sServiceStatus;
static SERVICE_STATUS_HANDLE s_hStatusHandler = (SERVICE_STATUS_HANDLE) NULL;
static void (WINAPI *s_lpfnUserPrintUsageFunc)() = base_print_usage_func;

////////////////////////////////////////////////////////////////////////////////////////
// static utility routines
////////////////////////////////////////////////////////////////////////////////////////
// Print out service error information
static void PrintErrorMessage(const char *lpszFormat)
{
	char *msg;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(char *)&msg, 0, NULL);
	printf(lpszFormat, s_szServiceName, GetLastError(), msg);
	LocalFree(msg);
}

// Get quotation format string if string contains space
static char *GetQuotationString(char *lpszBuffer)
{
	int len;
	if (strchr(lpszBuffer, ' ') == NULL) return lpszBuffer;
	len = strlen(lpszBuffer) + 1;
	if (len + 2 > sizeof(s_szStringBuffer)) len = sizeof(s_szStringBuffer) - 2;
	memmove(s_szStringBuffer + 1, lpszBuffer, len);
	s_szStringBuffer[0] = '\"';
	s_szStringBuffer[len] = '\"';
	s_szStringBuffer[len + 1] = 0;
	return s_szStringBuffer;
}

// get program's full path and file name in quotation format
static char *GetProgramFilename()
{
	// get program file path
	GetModuleFileName(NULL, s_szStringBuffer, sizeof(s_szStringBuffer));
	// add "" if program contains space
	return GetQuotationString(s_szStringBuffer);
}

// get program's path, without file name and extension
static char *GetProgramPath()
{
	char drive[100] = {0};
	char path[2000] = {0};
	_splitpath(GetProgramFilename(), drive, path, NULL, NULL);
	_snprintf(s_szStringBuffer, sizeof(s_szStringBuffer), "%s%s", drive, path); 
	s_szStringBuffer[sizeof(s_szStringBuffer) - 1] = 0;
	return s_szStringBuffer;
}

// get program's name, without path and extension
static char *GetProgramName()
{
	_splitpath(GetProgramFilename(), NULL, NULL, s_szStringBuffer, NULL);
	return s_szStringBuffer;
}

////////////////////////////////////////////////////////////////////////////////////////
// static callback functions
////////////////////////////////////////////////////////////////////////////////////////
// Callback function for console handler
static BOOL WINAPI ConsoleHandler(DWORD dwEvent)
{
	switch (dwEvent)
	{
	case CTRL_C_EVENT:
	case CTRL_BREAK_EVENT:
	case CTRL_CLOSE_EVENT:
		stop_service();
		return TRUE;
	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
	default:
		break;
	}
	return FALSE;
}

// Callback function for service control
static void WINAPI ControlHandler(DWORD dwOpcode)
{
	switch (dwOpcode)
	{
	case SERVICE_CONTROL_STOP:
	case SERVICE_CONTROL_SHUTDOWN:
		s_sServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
		SetServiceStatus(s_hStatusHandler, &s_sServiceStatus);
		stop_service();
		break;
	case SERVICE_CONTROL_PAUSE:
		break;
	case SERVICE_CONTROL_CONTINUE:
		break;
	case SERVICE_CONTROL_INTERROGATE:
		break;
	default:
		break;
	}
	// Report current status
	SetServiceStatus(s_hStatusHandler, &s_sServiceStatus);
}

// callback function for service main
static void WINAPI ServiceMain(DWORD argc, LPTSTR* argv)
{
	HWINSTA station;
	USEROBJECTFLAGS info;
	DWORD info_size;
	// Register the control request handler
    s_sServiceStatus.dwServiceType        = SERVICE_WIN32; 
    s_sServiceStatus.dwCurrentState       = SERVICE_START_PENDING; 
    s_sServiceStatus.dwControlsAccepted   = SERVICE_ACCEPT_STOP; 
    s_sServiceStatus.dwWin32ExitCode      = 0; 
    s_sServiceStatus.dwServiceSpecificExitCode = 0; 
    s_sServiceStatus.dwCheckPoint         = 0; 
    s_sServiceStatus.dwWaitHint           = 0; 
	s_hStatusHandler = RegisterServiceCtrlHandler(s_szServiceName, ControlHandler);
	if (s_hStatusHandler == (SERVICE_STATUS_HANDLE) NULL) return;

	// Start the initialization
	s_sServiceStatus.dwCurrentState       = SERVICE_RUNNING;
	s_sServiceStatus.dwWin32ExitCode      = 0;
	s_sServiceStatus.dwCheckPoint         = 0;
	s_sServiceStatus.dwWaitHint           = 0;
	SetServiceStatus(s_hStatusHandler, &s_sServiceStatus);

	// Init environment, which include current director, console, .etc
	SetCurrentDirectory(GetProgramPath());
	station = GetProcessWindowStation();
	GetUserObjectInformation(station, UOI_FLAGS, &info, sizeof(info), &info_size);
	if (info.dwFlags & WSF_VISIBLE)
	{
		AllocConsole();
		freopen("CONOUT$", "w+t", stdout);
		freopen("CONOUT$", "w+t", stderr);
		freopen("CONIN$",  "r+t", stdin);
		SetConsoleCtrlHandler(ConsoleHandler, TRUE);
	}
	// Run service process function
	if (s_dwRuntimeArgc <= 0 || s_lpRuntimeArgv == NULL) 
	{
		s_dwRuntimeArgc = argc;
		s_lpRuntimeArgv = argv;
	}
	run_service(s_dwRuntimeArgc, s_lpRuntimeArgv);

	// Tell the service manager we are stopped
	if (info.dwFlags & WSF_VISIBLE)
	{
		FreeConsole();
	}
	s_sServiceStatus.dwCurrentState = SERVICE_STOPPED;
	SetServiceStatus(s_hStatusHandler, &s_sServiceStatus);
}

////////////////////////////////////////////////////////////////////////////////////////
// service framework functions
////////////////////////////////////////////////////////////////////////////////////////
int WINAPI is_service()
{
	return s_bIsService;
}

void WINAPI set_service_name(const char* lpszServiceName)
{
	SAFE_COPY(s_szServiceName, lpszServiceName);
}

const char * WINAPI get_service_name()
{
	return s_szServiceName;
}

void WINAPI set_service_options(const char* lpszDisplayName,
								int         bInteractiveDesktop,
								int         bAutoStart,
								const char* lpszLoadOrderGroup,
								const char* lpszDependencyGroup,
								const char* lpszStartUsername,
								const char* lpszStartPassword)
{
	SAFE_COPY(s_szDisplayName, lpszDisplayName);
	SAFE_COPY(s_szLoadOrderGroup, lpszLoadOrderGroup);
	SAFE_COPY(s_szDependencyGroup, lpszDependencyGroup);
	SAFE_COPY(s_szStartUsername, lpszStartUsername);
	SAFE_COPY(s_szStartPassword, lpszStartPassword);
	s_dwServiceType = bInteractiveDesktop ? SERVICE_WIN32_OWN_PROCESS|SERVICE_INTERACTIVE_PROCESS : SERVICE_WIN32_OWN_PROCESS;
	s_dwStartType = bAutoStart ? SERVICE_AUTO_START : SERVICE_DEMAND_START;
}

void WINAPI set_print_usage_func(void (WINAPI *lpfnUserPrintUsage)())
{
	s_lpfnUserPrintUsageFunc = lpfnUserPrintUsage;
}

void WINAPI base_print_usage_func()
{
	printf("Usage:\n");
	printf("    %s [optional_options] mandatory_options [runtime_options]\n", GetProgramName());
	printf("Where:\n");
	printf("  Service optional options:\n");
	printf("    -name=NAME      set service name.\n");
	printf("    -disp=NAME      set service display name.\n");
	printf("    -interact=0|1   set service can interact with the desktop. (default: %s)\n", (s_dwServiceType & SERVICE_INTERACTIVE_PROCESS ? "1" : "0"));
	printf("    -autostart=0|1  set service can auto start. (default: %s)\n", (s_dwStartType == SERVICE_AUTO_START ? "1" : "0"));
	printf("    -order=ORDER    set service load order's group name.\n");
	printf("    -depend=DEPEND  set service load dependency's service name.\n");
	printf("    -user=USERNAME  set service execution's account name.\n");
	printf("    -pass=PASSWORD  set service execution's account password.\n");
	printf("\n");
	printf("    Notes: Use \"\" to enclose options with spaces.\n");
	printf("\n");
	printf("  Service mandatory options:\n");
	printf("    -i              install the service.\n");
	printf("    -u              uninstall the service.\n");
	printf("    -s              start the service.\n");
	printf("    -k              stop the service.\n");
	printf("    -v              display service information.\n");
	printf("    -d              debug the service (run as console).\n");
	printf("\n");
}

// Entry for CNTService, parsing command line arguments
int WINAPI enter_service(int argc, char* argv[])
{
	const char *error_msg = NULL;
	const char *service_name = s_szServiceName;
	const char *display_name = s_szDisplayName;
	const char *load_order = s_szLoadOrderGroup;
	const char *load_dependency = s_szDependencyGroup;
	const char *user_name = s_szStartUsername;
	const char *user_pass = s_szStartPassword;
	int service_type = s_dwServiceType;
	int start_type = s_dwStartType;
	int arg_index, operation;
	char *p, *p1, *p2;
	QUERY_SERVICE_CONFIG *config;
	SERVICE_TABLE_ENTRY st[] = { {s_szServiceName, ServiceMain}, {NULL, NULL} };
	// See if we have any command line arguments we recognize
	if (argc <= 1) goto DisplayHelp;

	// parse for service optional options from command line arguments
	error_msg = "Error: Invalid service options.\n\n";
	for (arg_index = 1; ; arg_index++)
	{
		if (arg_index >= argc) goto DisplayHelp;
		#define	MATCH_CMD(argv, cmd)	strnicmp(argv, cmd, strlen(cmd))
		p = argv[arg_index];
		if (MATCH_CMD(p, "-name=") == 0)
		{
			service_name = strchr(p, '=') + 1;
		}
		else if (MATCH_CMD(p, "-disp=") == 0)
		{
			display_name = strchr(p, '=') + 1;
		}
		else if (MATCH_CMD(p, "-interact=") == 0)
		{
			p = strchr(p, '=') + 1;
			service_type = atoi(p) ? SERVICE_WIN32_OWN_PROCESS|SERVICE_INTERACTIVE_PROCESS : SERVICE_WIN32_OWN_PROCESS;
		}
		else if (MATCH_CMD(p, "-autostart=") == 0)
		{
			p = strchr(p, '=') + 1;
			start_type = atoi(p) ? SERVICE_AUTO_START : SERVICE_DEMAND_START;
		}
		else if (MATCH_CMD(p, "-order=") == 0)
		{
			load_order = strchr(p, '=') + 1;
		}
		else if (MATCH_CMD(p, "-depend=") == 0)
		{
			load_dependency = strchr(p, '=') + 1;
		}
		else if (MATCH_CMD(p, "-user=") == 0)
		{
			p = strchr(p, '=') + 1;
			if (stricmp(p, "LocalService") == 0) user_name = "NT AUTHORITY\\LocalService";
			else if (stricmp(p, "NetworkService") == 0) user_name = "NT AUTHORITY\\NetworkService";
			else if (strchr(p, '\\') == NULL) user_name = (const char *) memcpy(p - 2, ".\\", 2);
			else user_name = p;
		}
		else if (MATCH_CMD(p, "-pass=") == 0)
		{
			user_pass = strchr(p, '=') + 1;
		}
		else if (stricmp(p, "-i") == 0 || stricmp(p, "-u") == 0 || stricmp(p, "-s") == 0 || stricmp(p, "-k") == 0 
			  || stricmp(p, "-v") == 0 || stricmp(p, "-d") == 0 || stricmp(p, "-r") == 0)
		{
			operation = p[1];
			arg_index++;
			break;
		}
		else 
		{
			goto DisplayHelp;
		}
	}
	// Verify service name is not null
	error_msg = "Error: Null name, must be specify in code or command line options.\n\n";
	if (strlen(service_name) == 0) goto DisplayHelp;
	set_service_name(service_name);
	if (strlen(display_name) == 0) display_name = NULL;
	if (strlen(load_order) == 0) load_order = NULL;
	if (strlen(load_dependency) == 0) load_dependency = NULL;
	if (strlen(user_name) == 0) user_name = NULL;
	if (strlen(user_pass) == 0) user_pass = NULL;
	// install/uninstall/debug/run service according to service mandatory options
	switch(operation)
	{
	case 'v':
		// Print out service info
		switch(GetServiceStatus(s_szServiceName))
		{
		case -1:
			PrintErrorMessage("The service '%s' failed to get status.\nError: %d, description: %s\n");
			return 0;
		case 0:
			printf("The service '%s' is not installed.\n", s_szServiceName);
			return 0;
		case SERVICE_CONTINUE_PENDING:
			printf("The service '%s' is installed and continue is pending.\n", s_szServiceName);
			break;
		case SERVICE_PAUSE_PENDING:
			printf("The service '%s' is installed and pause is pending.\n", s_szServiceName);
			break;
		case SERVICE_PAUSED:
			printf("The service '%s' is installed and is paused.\n", s_szServiceName);
			break;
		case SERVICE_RUNNING:
			printf("The service '%s' is installed and is running.\n", s_szServiceName);
			break;
		case SERVICE_START_PENDING:
			printf("The service '%s' is installed and is starting.\n", s_szServiceName);
			break;
		case SERVICE_STOP_PENDING:
			printf("The service '%s' is installed and is stopping.\n", s_szServiceName);
			break;
		case SERVICE_STOPPED:
			printf("The service '%s' is installed and is not running.\n", s_szServiceName);
			break;
		}
		config = GetServiceInfo(s_szServiceName);
		if (config == NULL)
		{
			PrintErrorMessage("The service '%s' failed to get service config.\nError: %d, description: %s\n");
			break;
		}
		switch (config->dwServiceType)
		{
		case SERVICE_KERNEL_DRIVER: 
 			p1 = "Driver service";
			break;
		case SERVICE_FILE_SYSTEM_DRIVER:
			p1 = "File system driver service";
			break;
		default:
			if (config->dwServiceType & SERVICE_INTERACTIVE_PROCESS) p1 = "Desktop interactive service";
			else p1 = "Normal service";
			break;
		}
		switch (config->dwStartType)
		{
		case SERVICE_AUTO_START:
			p2 = "Auto start"; 
			break; 
		case SERVICE_BOOT_START:
			p2 = "Boot start";
			break;
		case SERVICE_DEMAND_START:
			p2 = "Start on demand";
			break;
		case SERVICE_DISABLED:
			p2 = "Service disabled";
			break;
		case SERVICE_SYSTEM_START:
			p2 = "System start";
			break;
		default:
			p2 = "Unknown";
			break;
		}

		printf("Service config:\n");
		printf("  Service name     : %s\n", s_szServiceName);
		printf("  Display name     : %s\n", config->lpDisplayName ? config->lpDisplayName : "");
		printf("  Service type     : %s\n", p1);
		printf("  Start type       : %s\n", p2);
		printf("  Load order group : %s\n", config->lpLoadOrderGroup ? config->lpLoadOrderGroup : "");
		printf("  Dependency group : %s\n", config->lpDependencies ? config->lpDependencies : "");
		printf("  Start user name  : %s\n", config->lpServiceStartName ? config->lpServiceStartName : "");
		printf("  Binary path      : %s\n", config->lpBinaryPathName ? config->lpBinaryPathName : "");

		free(config);
		break;
	case 'i':
		// Request to install.
		switch(GetServiceStatus(s_szServiceName))
		{
		case -1:
			PrintErrorMessage("The service '%s' failed to get status.\nError: %d, description: %s\n");
			break;
		case 0:
			_snprintf(s_szStringBuffer, sizeof(s_szStringBuffer), "%s -name=%s -r", GetProgramFilename(), s_szServiceName);
			s_szStringBuffer[sizeof(s_szStringBuffer) - 1] = 0;
			while (arg_index < argc)
			{
				_snprintf(s_szStringBuffer + strlen(s_szStringBuffer), sizeof(s_szStringBuffer) - strlen(s_szStringBuffer), " %s", GetQuotationString(argv[arg_index++]));
				s_szStringBuffer[sizeof(s_szStringBuffer) - 1] = 0;
			}
			if (InstallService(s_szServiceName, display_name, service_type, start_type, 
								s_szStringBuffer, load_order, load_dependency, user_name, user_pass))
			{
				printf("The service '%s' has been installed.\n", s_szServiceName);
			}
			else
			{
				PrintErrorMessage("The service '%s' failed to install.\nError: %d, description: %s\n");
			}
			break;
		default:
			printf("The service '%s' is already installed\n", s_szServiceName);
			break;
		}
		break;
	case 'u':
		// Request to uninstall.
		switch(GetServiceStatus(s_szServiceName))
		{
		case -1:
			PrintErrorMessage("The service '%s' failed to get status.\nError: %d, description: %s\n");
			break;
		case 0:
			printf("The service '%s' is not installed\n", s_szServiceName);
			break;
		default:
			if (UninstallService(s_szServiceName))
			{
				printf("The service '%s' has been uninstalled.\n", s_szServiceName);
				printf("(You must delete the file \"%s\" yourself.)\n", GetProgramFilename());
			}
			else
			{
				PrintErrorMessage("The service '%s' failed to uninstall.\nError: %d, description: %s\n");
			}
			break;
		}
		break;
	case 's':
		// Request to start.
		switch(GetServiceStatus(s_szServiceName))
		{
		case -1:
			PrintErrorMessage("The service '%s' failed to get status.\nError: %d, description: %s\n");
			break;
		case 0:
			printf("The service '%s' is not installed\n", s_szServiceName);
			break;
		case SERVICE_STOPPED:
			if (StartExecuteService(s_szServiceName, SERVICE_WAIT_TIMEOUT))
			{
				printf("The service '%s' has been started.\n", s_szServiceName);
			}
			else
			{
				PrintErrorMessage("The service '%s' failed to start.\nError: %d, description: %s\n");
			}
			break;
		default:
			printf("The service '%s' is already started\n", s_szServiceName);
			break;
		}
		break;
	case 'k':
		// Request to stop.
		switch(GetServiceStatus(s_szServiceName))
		{
		case -1:
			PrintErrorMessage("The service '%s' failed to get status.\nError: %d, description: %s\n");
			break;
		case 0:
			printf("The service '%s' is not installed\n", s_szServiceName);
			break;
		case SERVICE_STOPPED:
			printf("The service '%s' is already stopped\n", s_szServiceName);
			break;
		default:
			if (StopExecuteService(s_szServiceName, SERVICE_WAIT_TIMEOUT))
			{
				printf("The service '%s' has been stopped.\n", s_szServiceName);
			}
			else
			{
				PrintErrorMessage("The service '%s' failed to stop.\nError: %d, description: %s\n");
			}
			break;
		}
		break;
	case 'd':
		// Request to debug.
		switch(GetServiceStatus(s_szServiceName))
		{
		case -1:
			PrintErrorMessage("The service '%s' failed to get status.\nError: %d, description: %s\n"
							  "We still continue to run in debug mode and maybe it will cause some error.\n");
			break;
		case 0:
		case SERVICE_STOPPED:
			break;
		default:
			printf("%s is installed and started, maybe you need stop service first.\n"
				   "We still continue to run in debug mode and maybe it will cause some error.\n", 
				   s_szServiceName);
			break;
		}
		SetConsoleCtrlHandler(ConsoleHandler, TRUE);
		s_bIsService = FALSE;
		argv[--arg_index] = argv[0];
		s_dwRuntimeArgc = argc - arg_index;
		s_lpRuntimeArgv = &argv[arg_index];
		return run_service(s_dwRuntimeArgc, s_lpRuntimeArgv);
		break;
	case 'r':
		s_bIsService = TRUE;
		argv[--arg_index] = argv[0];
		s_dwRuntimeArgc = argc - arg_index;
		s_lpRuntimeArgv = &argv[arg_index];
		return StartServiceCtrlDispatcher(st);
	}
	return 0;
DisplayHelp:
	if (error_msg) printf("%s", error_msg);
	if (s_lpfnUserPrintUsageFunc != NULL) (*s_lpfnUserPrintUsageFunc)();
	return ERROR_BAD_ARGUMENTS;
}

/// @endcond
