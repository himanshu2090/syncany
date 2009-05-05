/**	\file	NTServiceApp.cpp
	\brief	C++�汾��NT Service�����ȱʡmain������ʵ��

	����û�δʵ���Լ���main�������������ӵ�ʱ����Զ����Ӹ�ģ��
	\version	1.0		Initial release
	\author	���
	\date	2005-12-12
*/

#include <windows.h>
#include <stdio.h>
#include "NTService.h"
/**	\brief	C++�汾��NT Service�����ȱʡmain����
	
	����û�δʵ���Լ���main�������������ӵ�ʱ����Զ����Ӹ�ģ�顣�û�Ҳ�����Լ�ʵ��main
	������ʵ�ֵĵķ������£�
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
