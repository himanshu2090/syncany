/**	\file	NTService.cpp
	\brief	C++版本的NT Service框架类CNTService的实现
	\version	1.0		Initial release
	\author	葛兵
	\date	2005-12-12
*/

#include "NTServiceUtils.h"
#include "NTService.h"

///	@cond invisible
////////////////////////////////////////////////////////////////////////////////////////
// NTServiceCore implement functions
////////////////////////////////////////////////////////////////////////////////////////
static CNTService *s_lpNTService = NULL;
int WINAPI run_service(int argc, char* argv[])
{
	if (s_lpNTService != NULL) return s_lpNTService->RunService(argc, argv);
	return 0;
}

void WINAPI stop_service()
{
	if (s_lpNTService != NULL) s_lpNTService->StopService();
}

void WINAPI user_print_usage_func()
{
	if (s_lpNTService != NULL) s_lpNTService->PrintUsage();
}

#include "NTServiceCore.h"

////////////////////////////////////////////////////////////////////////////////////////
// CNTservice implement functions
////////////////////////////////////////////////////////////////////////////////////////
// Constructor/Destructor for CNTService
CNTService::CNTService(const char* lpszServiceName,
					   const char* lpszDisplayName,
					   bool        bInteractiveDesktop,
					   bool        bAutoStart,
					   const char* lpszLoadOrderGroup,
					   const char* lpszDependencyGroup,
					   const char* lpszStartUsername,
					   const char* lpszStartPassword)
{
	s_lpNTService = this;
	set_print_usage_func(user_print_usage_func);
	set_service_name(lpszServiceName);
	set_service_options(lpszDisplayName, bInteractiveDesktop, bAutoStart, lpszLoadOrderGroup, lpszDependencyGroup, lpszStartUsername, lpszStartPassword);
}

CNTService::~CNTService()
{
	s_lpNTService = NULL;
}

////////////////////////////////////////////////////////////////////////////////////////
// Get/Set service attribute
////////////////////////////////////////////////////////////////////////////////////////
CNTService *CNTService::GetServiceInstance()
{
	return s_lpNTService;
}

void CNTService::SetServiceName(const char* lpszServiceName)
{
	set_service_name(lpszServiceName);
}

const char *CNTService::GetServiceName()
{
	return get_service_name();
}

bool CNTService::IsService()
{
	return is_service() ? true : false;
}

void CNTService::SetServiceOptions(const char* lpszDisplayName,
								   bool        bInteractiveDesktop,
								   bool        bAutoStart,
								   const char* lpszLoadOrderGroup,
								   const char* lpszDependencyGroup,
								   const char* lpszStartUsername,
								   const char* lpszStartPassword)
{
	set_service_options(lpszDisplayName, bInteractiveDesktop, bAutoStart, lpszLoadOrderGroup, lpszDependencyGroup, lpszStartUsername, lpszStartPassword);
}

////////////////////////////////////////////////////////////////////////////////////////
// Service internal entry point
////////////////////////////////////////////////////////////////////////////////////////
int CNTService::EnterService(int argc, char* argv[])
{
	return enter_service(argc, argv);
}

////////////////////////////////////////////////////////////////////////////////////////
///	Derived class may override
////////////////////////////////////////////////////////////////////////////////////////
void CNTService::PrintUsage()
{
	base_print_usage_func();
}

/// @endcond
