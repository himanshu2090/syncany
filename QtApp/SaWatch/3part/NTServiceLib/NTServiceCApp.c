/**	\file	NTServiceCApp.c
	\brief	C版本的NT Service框架类缺省main函数的实现

	如果用户未实现自己的main函数，则在链接的时候会自动链接该模块
	\version	1.0		Initial release
	\author	葛兵
	\date	2005-12-12
*/

#include <windows.h>
#include "NTServiceC.h"

/**	\brief	C版本的NT Service框架下缺省main函数
	
	如果用户未实现自己的main函数，则在链接的时候会自动链接该模块。用户也可以自己实现main
	函数，实现的的方法如下：
	\code
	#include <NTService.h>
	int main(int argc, char* argv[])
	{
		int rt;
		init_service();
		rt = enter_service(argc, argv);
		exit_service();
		return rt;
	}
	\endcode
*/
int main(int argc, char* argv[])
{
	int rt;
	init_service();
	rt = enter_service(argc, argv);
	exit_service();
	return rt;
}
