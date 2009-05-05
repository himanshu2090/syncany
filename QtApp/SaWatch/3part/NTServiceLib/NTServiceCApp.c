/**	\file	NTServiceCApp.c
	\brief	C�汾��NT Service�����ȱʡmain������ʵ��

	����û�δʵ���Լ���main�������������ӵ�ʱ����Զ����Ӹ�ģ��
	\version	1.0		Initial release
	\author	���
	\date	2005-12-12
*/

#include <windows.h>
#include "NTServiceC.h"

/**	\brief	C�汾��NT Service�����ȱʡmain����
	
	����û�δʵ���Լ���main�������������ӵ�ʱ����Զ����Ӹ�ģ�顣�û�Ҳ�����Լ�ʵ��main
	������ʵ�ֵĵķ������£�
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
