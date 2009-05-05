/**	\file		NTServiceC.h
	\brief		C版本的NT Service框架的定义
	\version	1.0		Initial release
	\author		葛兵
	\date		2005-12-12
*/

#ifndef _NTSERVICE_C_H_
#define _NTSERVICE_C_H_

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup NTServiceC C版本的NT Service框架

	用户只需要实现NT Service框架必须实现的函数，就可以方便的实现NT Service的功能，包
	括安装/卸载/启动/停止服务、显示服务状态、以命令行方式运行服务程序等。用户还可以
	在命令行中控制服务名称、显示名称、是否与桌面交互、启动方式、启动的顺序、依赖的服
	务、启动的用户/密码等参数。\n
	\n
	示例代码:\n
	\code
	#include <NTServiceC.h>
	// 实现NT Service框架必须实现的函数
	void WINAPI init_service()
	{
		set_service_name("MyService");
		...
	}
	void WINAPI exit_service()
	{
		...
	}
	int WINAPI run_service(int argc, char* argv[])
	{
		...
	}
	void WINAPI stop_service()
	{
		...
	}
	// 编写main函数并调用函数init_service, enter_service, exit_service
	// 注：本步骤并不是必须的，如果用户未定义自己的main函数，则在Link的时间会
	// 自动连接连接Lib中包含main函数的模块
	int main(int argc, char* argv[])
	{
		int rt;
		init_service();
		rt = enter_service(argc, argv);
		exit_service();
		return rt;
	}
	\endcode
	\{
*/

/**	\brief	指向打印命令行参数帮助的函数的指针
*/
typedef void (WINAPI *LP_PRINT_USAGE_FUNC)();

//////////////////////////////////////////////////////////////////////////
/// @name 设置和获得服务属性的函数
///	@{
//////////////////////////////////////////////////////////////////////////
/**	\brief	程序目前是否运行在服务状态

	\return	1 - 程序目前运行在服务状态；0 - 程序目前运行在命令行状态
*/
int WINAPI is_service();
/** \brief	设置服务的名称

	\param	lpszServiceName		服务的名称
*/
void WINAPI set_service_name(const char* lpszServiceName);
/**	\brief	得到服务的名称

	\return	服务的名称
*/
const char * WINAPI get_service_name();
/**	\brief	设置服务的缺省参数

	\param	lpszDisplayName		服务显示的名称。如果为空，则与服务名称一致
	\param	bInteractiveDesktop	服务是否可以与桌面交互
	\param	bAutoStart			服务是否自动启动
	\param	lpszLoadOrderGroup	服务自动启动的顺序组的名称。如果为空，则服务自动启动无顺序
	\param	lpszDependencyGroup	服务启动时依赖的服务或服务组。如果为空，则服务不依赖于其他服务
	\param	lpszStartUsername	服务启动时的用户帐户。如果为空，则服务以LocalSystem启动
	\param	lpszStartPassword	服务启动时的用户密码。只要在用户帐户非空时才有效
*/
void WINAPI set_service_options(const char* lpszDisplayName,
								int         bInteractiveDesktop,
								int         bAutoStart,
								const char* lpszLoadOrderGroup,
								const char* lpszDependencyGroup,
								const char* lpszStartUsername,
								const char* lpszStartPassword);
/**	\brief	设置用户定义的打印命令行参数帮助的函数

	如果为NULL，则关闭打印命令行参数帮助的功能。
	\param	lpfnUserPrintUsage	指向用户定义的打印命令行参数帮助的函数的指针
*/
void WINAPI set_print_usage_func(LP_PRINT_USAGE_FUNC lpfnUserPrintUsage);
/**	\brief	缺省的打印命令行参数帮助的函数

	打印出通过命令行设置服务的名称和参数的帮助信息，以及通过命令行安装、卸载、启动、停止
	和调试服务的帮助信息
*/
void WINAPI base_print_usage_func();
///	@}

//////////////////////////////////////////////////////////////////////////
/// @name 用户必须实现的函数
///	@{
//////////////////////////////////////////////////////////////////////////
/**	\brief	用户必须实现的函数，具体实现初始化NT Service框架的功能

	用户可以在该函数设置服务的缺省名称和缺省参数，以及相关资源的初始化。具体示例如下：
	\code
	void WINAPI init_service()
	{
		set_service_name("DemoService");
		set_service_options("Demo display name",	// display name
							0,						// no interactive with desktop
							1,						// auto start
							NULL,					// load order
							NULL,					// dependency group
							NULL,					// start account's username
							NULL					// start account's password
							);
	}
	\endcode
	\note	这些缺省参数都可以通过被程序的命令行所修改。
*/
void         WINAPI init_service();
/** \brief	用户必须实现的函数，实现具体运行服务的功能

	相当于普通Console程序的main()函数。用户只需要将原有main函数中的代码复制到run_service中即可。
	\note	此时的argc/argv与调用enter_service时的不同，此时的argc/argv已经去掉服务相关的命令行
	参数，只保留与程序运行相关的命令行参数。
	\param	argc				相当于普通Console程序的main函数的argc
	\param	argv[]				相当于普通Console程序的main函数的argv
	\return	程序执行的返回值
*/
int          WINAPI run_service(int argc, char* argv[]);	// 相当于普通Console程序的main()函数
/**	\brief	用户必须实现的函数，具体实现停止服务的功能

	用户需要在stop_service中实现停止run_service函数运行的功能。
*/
void         WINAPI stop_service();
/**	\brief	用户必须实现的函数，具体实现清除NT Service框架的功能

	用户可以在该函数中释放和关闭相关的资源。
*/
void         WINAPI exit_service();
///	@}

//////////////////////////////////////////////////////////////////////////
/// @name 内部入口函数
///	@{
//////////////////////////////////////////////////////////////////////////
/**	\brief	C版本的NT Service框架的的内部入口函数

	根据命令行参数安装/删除/启动/停止/调试服务，一般由main函数调用该函数。
	\param	argc				main函数的argc
	\param	argv[]				main函数的argv
	\return	程序执行的返回值
*/
int          WINAPI enter_service(int argc, char* argv[]);
/// @}

/*	\} */

#ifdef __cplusplus
}
#endif

// 自动连接NTServiceLibc
#ifdef _DEBUG
	#pragma comment(lib, "NTServiceLibcDebug.lib") 
	#pragma message("Automatically linking with NTServiceLibcDebug.lib")
#else
	#pragma comment(lib, "NTServiceLibc.lib") 
	#pragma message("Automatically linking with NTServiceLibc.lib")
#endif

#endif // _NTSERVICE_C_H_
