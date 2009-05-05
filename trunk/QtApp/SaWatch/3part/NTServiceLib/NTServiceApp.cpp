/**	\file	NTServiceApp.cpp
	\brief	C++版本的NT Service框架类缺省main函数的实现

	如果用户未实现自己的main函数，则在链接的时候会自动链接该模块
	\version	1.0		Initial release
	\author	葛兵
	\date	2005-12-12
*/

#include <windows.h>
#include <stdio.h>
#include "NTService.h"
/**	\brief	C++版本的NT Service框架下缺省main函数
	
	如果用户未实现自己的main函数，则在链接的时候会自动链接该模块。用户也可以自己实现main
	函数，实现的的方法如下：
	\code
	#include <NTService.h>
	int main(int argc, char* argv[])
	{
		CNTService *service = CNTService::GetServiceInstance();
		if (service == NULL) return ERROR_INVALID_FUNCTION;
		return service->EnterService(argc, argv);
	}
	\endcode
*/
int main(int argc, char* argv[])
{
	CNTService *service = CNTService::GetServiceInstance();
	if (service == NULL)
	{
		printf("Error: Service class derived from CNTService doesn't instance, please check your code.\n");
		return ERROR_INVALID_FUNCTION;
	}
	return service->EnterService(argc, argv);
}
