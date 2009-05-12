/**	\file		NTService.h
	\brief		C++版本的NT Service框架类CNTService的定义
	\version	1.0		Initial release
	\author		葛兵
	\date		2005-12-12
*/

#ifndef _NTSERVICE_H_
#define _NTSERVICE_H_

/**	\class	CNTService NTService.h "NTService.h"
	\brief	C++版本的NT Service框架类
	
	用户只需要定义自己的CNTService派生类，就可以方便的实现NT Service的功能，包括
	安装/卸载/启动/停止服务、显示服务状态、以命令行方式运行服务程序等。用户还可以
	在命令行中控制服务名称、显示名称、是否与桌面交互、启动方式、启动的顺序、依赖
	的服务、启动的用户/密码等参数。\n
	\n
	示例代码:\n
	\code
	#include <NTService.h>
	// 声明并实现CNTService的一个派生类MyNTService
	class MyNTService : public CNTService
	{
	public:
		MyNTService() : CNTService("MyService") 
		{
		}
		//  必须实现RunService
		int  RunService(int argc, char* argv[])
		{
			...
		}
		// 必须实现StopService
		void StopService()
		{
			...
		}
	};
	// 定义一个派生类MyNTService的实例
	MyNTService my_service;
	// 编写main函数并调用CNTService的EnterService()函数
	// 注：本步骤并不是必须的，如果用户未定义自己的main函数，则在Link的时间会
	// 自动连接连接Lib中包含main函数的模块
	int main(int argc, char* argv[])
	{
	    int rt = CNTService::GetServiceInstance()->EnterService(argc, argv);
	    return rt;
	}
	\endcode
*/
class CNTService
{
public:
	//////////////////////////////////////////////////////////////////////////
	/// @name 构造和析构函数
	///	@{
	//////////////////////////////////////////////////////////////////////////
	/**	\brief	CNTService的构造函数

		设置服务的名称及缺省的服务参数。这些参数都可以通过被程序的命令行所修改。

		\param	lpszServiceName		服务的名称。如果为空，则必须调用SetServiceName设置服务名称
		\param	lpszDisplayName		服务显示的名称。如果为空，则与服务名称一致
		\param	bInteractiveDesktop	服务是否可以与桌面交互
		\param	bAutoStart			服务是否自动启动
		\param	lpszLoadOrderGroup	服务自动启动的顺序组的名称。如果为空，则服务自动启动无顺序
		\param	lpszDependencyGroup	服务启动时依赖的服务或服务组。如果为空，则服务不依赖于其他服务
		\param	lpszStartUsername	服务启动时的用户帐户。如果为空，则服务以LocalSystem启动
		\param	lpszStartPassword	服务启动时的用户密码。只要在用户帐户非空时才有效
	*/
	CNTService(const char* lpszServiceName = NULL, 
			   const char* lpszDisplayName = NULL,
			   bool        bInteractiveDesktop = false,
			   bool        bAutoStart = true,
			   const char* lpszLoadOrderGroup = NULL,
			   const char* lpszDependencyGroup = NULL,
			   const char* lpszStartUsername = NULL,
			   const char* lpszStartPassword = NULL);
	/**	\brief	CNTService的析构函数

		无具体的操作，派生类可以在析构函数中释放和关闭相关的资源。
	*/
	virtual ~CNTService();
	///	@}

	//////////////////////////////////////////////////////////////////////////
	/// @name 设置和获得服务属性的函数
	///	@{
	//////////////////////////////////////////////////////////////////////////
	/** \brief	得到当前CNTService实例对象

		\return	当前CNTService实例对象的指针。NULL表示无CNTService实例，很可能是用户未定义派生类的示例
	*/
	static CNTService *GetServiceInstance();
	/**	\brief	程序目前是否运行在服务状态

		\return	true - 程序目前运行在服务状态；false - 程序目前运行在命令行状态
	*/
	bool IsService();
	/** \brief	设置服务的名称

		\param	lpszServiceName		服务的名称
	*/
	void SetServiceName(const char* lpszServiceName);
	/**	\brief	得到服务的名称

		\return	服务的名称
	*/
	const char *GetServiceName();
	/**	\brief	设置服务的缺省参数

		\param	lpszDisplayName		服务显示的名称。如果为空，则与服务名称一致
		\param	bInteractiveDesktop	服务是否可以与桌面交互
		\param	bAutoStart			服务是否自动启动
		\param	lpszLoadOrderGroup	服务自动启动的顺序组的名称。如果为空，则服务自动启动无顺序
		\param	lpszDependencyGroup	服务启动时依赖的服务或服务组。如果为空，则服务不依赖于其他服务
		\param	lpszStartUsername	服务启动时的用户帐户。如果为空，则服务以LocalSystem启动
		\param	lpszStartPassword	服务启动时的用户密码。只要在用户帐户非空时才有效
	*/
	void SetServiceOptions(const char* lpszDisplayName = NULL,
						   bool        bInteractiveDesktop = false,
						   bool        bAutoStart = true,
						   const char* lpszLoadOrderGroup = NULL,
						   const char* lpszDependencyGroup = NULL,
						   const char* lpszStartUsername = NULL,
						   const char* lpszStartPassword = NULL);
	///	@}

	//////////////////////////////////////////////////////////////////////////
	/// @name 派生类需要实现的函数
	///	@{
	//////////////////////////////////////////////////////////////////////////
	/** \brief	派生类必须实现的函数，具体实现运行服务的功能

		相当于普通Console程序的main()函数。用户只需要将原有main函数中的代码复制到RunService中即可。
		\note	此时的argc/argv与调用EnterService时的不同，此时的argc/argv已经去掉CNTService相关的
		命令行参数，只保留与程序运行相关的命令行参数。
		\param	argc				相当于普通Console程序的main函数的argc
		\param	argv[]				相当于普通Console程序的main函数的argv
		\return	程序执行的返回值
	*/
	virtual int  RunService(int argc, char* argv[]) = NULL;	// 
	/**	\brief	派生类必须实现的函数，具体实现停止服务的功能

		用户需要在StopService中实现停止RunService函数运行的功能。
	*/
	virtual void StopService() = NULL;
	/**	\brief	打印程序命令行参数的帮助

		用户定义的CNTService派生类可以重写该函数，也可以调用基类的函数后打印自己的命令行参数的帮助。
		一般的实现如下：
		\code
		void MyNTService::PrintUsage()
		{
			// 调用基类的函数打印服务的命令行帮助
			CNTService::PrintUsage();
			// 打印派生类程序RunService的命令行帮助
			printf(...);
		}
		\endcode
	*/
	virtual void PrintUsage();
	///	@}

	//////////////////////////////////////////////////////////////////////////
	/// @name 内部入口函数
	///	@{
	//////////////////////////////////////////////////////////////////////////
	/**	\brief	CNTService的内部入口函数
	
		根据命令行参数安装/删除/启动/停止/调试服务，派生类一般不需要重写该函数。
		\param	argc				main函数的argc
		\param	argv[]				main函数的argv
		\return	程序执行的返回值
	*/
	int EnterService(int argc, char* argv[]);
	///	@}
};

// 自动连接NTServiceLib
#ifdef _DEBUG
	//#pragma comment(lib, "NTServiceLibDebug.lib") 
	//#pragma message("Automatically linking with NTServiceLibDebug.lib")
#else
	//#pragma comment(lib, "NTServiceLib.lib") 
	//#pragma message("Automatically linking with NTServiceLib.lib")
#endif

#endif // _NTSERVICE_H_
