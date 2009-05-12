/**	\file		NTService.h
	\brief		C++�汾��NT Service�����CNTService�Ķ���
	\version	1.0		Initial release
	\author		���
	\date		2005-12-12
*/

#ifndef _NTSERVICE_H_
#define _NTSERVICE_H_

/**	\class	CNTService NTService.h "NTService.h"
	\brief	C++�汾��NT Service�����
	
	�û�ֻ��Ҫ�����Լ���CNTService�����࣬�Ϳ��Է����ʵ��NT Service�Ĺ��ܣ�����
	��װ/ж��/����/ֹͣ������ʾ����״̬���������з�ʽ���з������ȡ��û�������
	���������п��Ʒ������ơ���ʾ���ơ��Ƿ������潻����������ʽ��������˳������
	�ķ����������û�/����Ȳ�����\n
	\n
	ʾ������:\n
	\code
	#include <NTService.h>
	// ������ʵ��CNTService��һ��������MyNTService
	class MyNTService : public CNTService
	{
	public:
		MyNTService() : CNTService("MyService") 
		{
		}
		//  ����ʵ��RunService
		int  RunService(int argc, char* argv[])
		{
			...
		}
		// ����ʵ��StopService
		void StopService()
		{
			...
		}
	};
	// ����һ��������MyNTService��ʵ��
	MyNTService my_service;
	// ��дmain����������CNTService��EnterService()����
	// ע�������貢���Ǳ���ģ�����û�δ�����Լ���main����������Link��ʱ���
	// �Զ���������Lib�а���main������ģ��
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
	/// @name �������������
	///	@{
	//////////////////////////////////////////////////////////////////////////
	/**	\brief	CNTService�Ĺ��캯��

		���÷�������Ƽ�ȱʡ�ķ����������Щ����������ͨ������������������޸ġ�

		\param	lpszServiceName		��������ơ����Ϊ�գ���������SetServiceName���÷�������
		\param	lpszDisplayName		������ʾ�����ơ����Ϊ�գ������������һ��
		\param	bInteractiveDesktop	�����Ƿ���������潻��
		\param	bAutoStart			�����Ƿ��Զ�����
		\param	lpszLoadOrderGroup	�����Զ�������˳��������ơ����Ϊ�գ�������Զ�������˳��
		\param	lpszDependencyGroup	��������ʱ�����ķ��������顣���Ϊ�գ��������������������
		\param	lpszStartUsername	��������ʱ���û��ʻ������Ϊ�գ��������LocalSystem����
		\param	lpszStartPassword	��������ʱ���û����롣ֻҪ���û��ʻ��ǿ�ʱ����Ч
	*/
	CNTService(const char* lpszServiceName = NULL, 
			   const char* lpszDisplayName = NULL,
			   bool        bInteractiveDesktop = false,
			   bool        bAutoStart = true,
			   const char* lpszLoadOrderGroup = NULL,
			   const char* lpszDependencyGroup = NULL,
			   const char* lpszStartUsername = NULL,
			   const char* lpszStartPassword = NULL);
	/**	\brief	CNTService����������

		�޾���Ĳ���������������������������ͷź͹ر���ص���Դ��
	*/
	virtual ~CNTService();
	///	@}

	//////////////////////////////////////////////////////////////////////////
	/// @name ���úͻ�÷������Եĺ���
	///	@{
	//////////////////////////////////////////////////////////////////////////
	/** \brief	�õ���ǰCNTServiceʵ������

		\return	��ǰCNTServiceʵ�������ָ�롣NULL��ʾ��CNTServiceʵ�����ܿ������û�δ�����������ʾ��
	*/
	static CNTService *GetServiceInstance();
	/**	\brief	����Ŀǰ�Ƿ������ڷ���״̬

		\return	true - ����Ŀǰ�����ڷ���״̬��false - ����Ŀǰ������������״̬
	*/
	bool IsService();
	/** \brief	���÷��������

		\param	lpszServiceName		���������
	*/
	void SetServiceName(const char* lpszServiceName);
	/**	\brief	�õ����������

		\return	���������
	*/
	const char *GetServiceName();
	/**	\brief	���÷����ȱʡ����

		\param	lpszDisplayName		������ʾ�����ơ����Ϊ�գ������������һ��
		\param	bInteractiveDesktop	�����Ƿ���������潻��
		\param	bAutoStart			�����Ƿ��Զ�����
		\param	lpszLoadOrderGroup	�����Զ�������˳��������ơ����Ϊ�գ�������Զ�������˳��
		\param	lpszDependencyGroup	��������ʱ�����ķ��������顣���Ϊ�գ��������������������
		\param	lpszStartUsername	��������ʱ���û��ʻ������Ϊ�գ��������LocalSystem����
		\param	lpszStartPassword	��������ʱ���û����롣ֻҪ���û��ʻ��ǿ�ʱ����Ч
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
	/// @name ��������Ҫʵ�ֵĺ���
	///	@{
	//////////////////////////////////////////////////////////////////////////
	/** \brief	���������ʵ�ֵĺ���������ʵ�����з���Ĺ���

		�൱����ͨConsole�����main()�������û�ֻ��Ҫ��ԭ��main�����еĴ��븴�Ƶ�RunService�м��ɡ�
		\note	��ʱ��argc/argv�����EnterServiceʱ�Ĳ�ͬ����ʱ��argc/argv�Ѿ�ȥ��CNTService��ص�
		�����в�����ֻ���������������ص������в�����
		\param	argc				�൱����ͨConsole�����main������argc
		\param	argv[]				�൱����ͨConsole�����main������argv
		\return	����ִ�еķ���ֵ
	*/
	virtual int  RunService(int argc, char* argv[]) = NULL;	// 
	/**	\brief	���������ʵ�ֵĺ���������ʵ��ֹͣ����Ĺ���

		�û���Ҫ��StopService��ʵ��ֹͣRunService�������еĹ��ܡ�
	*/
	virtual void StopService() = NULL;
	/**	\brief	��ӡ���������в����İ���

		�û������CNTService�����������д�ú�����Ҳ���Ե��û���ĺ������ӡ�Լ��������в����İ�����
		һ���ʵ�����£�
		\code
		void MyNTService::PrintUsage()
		{
			// ���û���ĺ�����ӡ����������а���
			CNTService::PrintUsage();
			// ��ӡ���������RunService�������а���
			printf(...);
		}
		\endcode
	*/
	virtual void PrintUsage();
	///	@}

	//////////////////////////////////////////////////////////////////////////
	/// @name �ڲ���ں���
	///	@{
	//////////////////////////////////////////////////////////////////////////
	/**	\brief	CNTService���ڲ���ں���
	
		���������в�����װ/ɾ��/����/ֹͣ/���Է���������һ�㲻��Ҫ��д�ú�����
		\param	argc				main������argc
		\param	argv[]				main������argv
		\return	����ִ�еķ���ֵ
	*/
	int EnterService(int argc, char* argv[]);
	///	@}
};

// �Զ�����NTServiceLib
#ifdef _DEBUG
	//#pragma comment(lib, "NTServiceLibDebug.lib") 
	//#pragma message("Automatically linking with NTServiceLibDebug.lib")
#else
	//#pragma comment(lib, "NTServiceLib.lib") 
	//#pragma message("Automatically linking with NTServiceLib.lib")
#endif

#endif // _NTSERVICE_H_
