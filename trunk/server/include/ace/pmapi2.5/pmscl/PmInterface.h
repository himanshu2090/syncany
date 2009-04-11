#ifndef __PM_INTERFACE_H__
#define __PM_INTERFACE_H__

/**	
 *@file PmInterface.h 
 *@brief pm接口
*/

/**	\brief	Redefinition of PM_DWORD as PM_IID
*/
typedef PM_DWORD PM_IID;
/**	\brief	The IPmUnknown struct

*/
#define IID_IPmUnknown		0x00000000

/**	\brief	The IPmUnknown struct

*/
struct IPmUnknown
{
	virtual PM_LONG release()=0;
	virtual PM_LONG addRef()=0;
	virtual PM_BOOL queryInterface( PM_IID iid,void** ppUnknown )=0;
};
#ifdef __cplusplus
extern "C"
{
#endif

/**	\brief	The pmcom_initialize function


	\return	PM_BOOL

	
*/
PM_BOOL pmcom_initialize();
/**	\brief	The pmcom_uninitialize function


	\return	PM_VOID

	
*/
PM_VOID pmcom_uninitialize();
/**	\brief	The pmcom_register_class function

	\param	pszcClassName	a parameter of type PM_LPCSTR
	\param	dwVer	a parameter of type PM_DWORD
	\param	pszPath	a parameter of type PM_LPCSTR

	\return	PM_BOOL

	
*/
PM_BOOL pmcom_register_class( PM_LPCSTR pszcClassName, PM_DWORD dwVer, PM_LPCSTR pszPath );
/**	\brief	The pmcom_unregister_class function

	\param	pszcClassName	a parameter of type PM_LPCSTR
	\param	dwVer	a parameter of type PM_DWORD

	\return	PM_BOOL

	
*/
PM_BOOL pmcom_unregister_class( PM_LPCSTR pszcClassName, PM_DWORD dwVer );
/**	\brief	The pmcom_create_instance function
	\param	pszName	a parameter of type PM_LPCSTR
	\param	ppIterface	a parameter of type IPmUnknown**
	\param	dwContext	a parameter of type PM_DWORD , 创建object时需要的上下文信息
	\return	PM_BOOL
	\note ppInterface指向的内容将被release，除非*pInterface的内容为NULL
*/
PM_BOOL pmcom_create_instance( PM_LPCSTR pszName, IPmUnknown** ppIterface, PM_DWORD dwContext );
/**	\brief pmcom_inc
*/
PM_LONG pmcom_inc( PM_LONG* );
/**	\brief pmcom_dec
*/
PM_LONG pmcom_dec( PM_LONG* );
/**	\brief	The pmcom_set_lib_path function

	\param	szLibPath	a parameter of type PM_LPCSTR

	\return	PM_VOID

	
*/
PM_VOID pmcom_set_lib_path(PM_LPCSTR szLibPath);
PM_VOID pmcom_set_register_file_name(PM_LPCSTR szFileName);

#ifdef __cplusplus
}
#endif

//使用如下宏，需要包含头文件定义如下
//#ifdef WIN32
//	#include <windows.h>
//#else
//	#include<dlfcn.h>
//#endif

#ifdef WIN32
/**	\brief PM_INTERFACE_DELCARE_PATH
*/
#define PM_INTERFACE_DELCARE_PATH \
	char g_szDllPath[PM_MAX_PATH]={0};\
	HINSTANCE g_hDll = NULL;\
	BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved )\
	{\
		switch( fdwReason )\
		{\
		case DLL_PROCESS_ATTACH:\
			g_hDll = hinstDLL;\
			memset( g_szDllPath,0,sizeof(g_szDllPath));\
			GetModuleFileName( g_hDll, g_szDllPath, sizeof(g_szDllPath) );\
			::CoInitialize(NULL);break;\
			break;\
		case DLL_PROCESS_DETACH: ::CoUninitialize();break;\
		case DLL_THREAD_ATTACH:break;\
		case DLL_THREAD_DETACH:break;\
		}\
		return TRUE;\
	}\
	const char* pmcom_get_dll_path()\
	{\
		return g_szDllPath;\
	}
#else
/**	\brief	PM_INTERFACE_DELCARE_PATH

*/
#define PM_INTERFACE_DELCARE_PATH \
	char g_szDllPath[PM_MAX_PATH]={0};\
	const char* pmcom_get_dll_path()\
	{\
		if( g_szDllPath[0] == '\0' ) \
		{\
			Dl_info dll_info;\
			memset(&dll_info,0,sizeof(dll_info));\
			dladdr(g_szDllPath,&dll_info);\
			if(!dll_info.dli_fname) return PM_NULL;\
			memset( g_szDllPath, 0, sizeof(g_szDllPath));\
			strncpy(g_szDllPath,dll_info.dli_fname,sizeof(g_szDllPath)-1);\
		}\
		return g_szDllPath;\
	}
#endif

/**	\brief	The PM_REF_COUNT struct

*/
struct PM_REF_COUNT
{
	long lRef;
	PM_REF_COUNT():lRef(0){}
	long addRef(){
		return pmcom_inc(&lRef);
	}
	long release(){
		return pmcom_dec(&lRef);
	}
};

/**	\brief PMCOM_BEGIN_MAP
*/
#define PMCOM_BEGIN_MAP(defaultType)                                                                 \
	private:                                                                                         \
		PM_REF_COUNT m_objRef;                                                                       \
	public:                                                                                          \
	virtual long addRef(){ return m_objRef.addRef(); }	                                             \
		virtual long release(){ long ret = m_objRef.release();if( ret<=0 ) delete this; return ret; }\
		virtual PM_BOOL queryInterface( PM_IID iid,void** ppvUnknown ){                              \
			if( ppvUnknown == PM_NULL ) return PM_FALSE;                                             \
			IPmUnknown** ppUnknown = (IPmUnknown**)ppvUnknown;                                       \
			*ppUnknown = PM_NULL;                                                                    \
			if( iid == IID_IPmUnknown )	{                                                            \
				*ppUnknown =  static_cast<defaultType*>(this);                                       \
				(*ppUnknown)->addRef();                                                              \
				return PM_TRUE;                                                                      \
			}
/**	\brief PMCOM_CHAIN_INTERFACE
*/
#define PMCOM_CHAIN_INTERFACE( iname ) 	else if( iid == IID_##iname )	{\
		*ppUnknown = static_cast<iname*>(this);\
		(*ppUnknown)->addRef();\
		return PM_TRUE;\
	}
/**	\brief PMCOM_CHAIN_INTERFACE2
*/
#define PMCOM_CHAIN_INTERFACE2( iname,tmptype ) 	else if( iid == IID_##iname )	{\
		*ppUnknown = static_cast<iname*>( ( static_cast<tmptype*> (this) ) );\
		(*ppUnknown)->addRef();\
		return PM_TRUE;\
	}
/**	\brief PMCOM_END_MAP
*/
#define PMCOM_END_MAP return PM_FALSE;}                                                              \
	PmUnknownPtr getBase(){                                                                          \
		PmUnknownPtr ptrUnknown;                                                                     \
		if( !queryInterface(IID_IPmUnknown, (void**)&ptrUnknown))                                    \
			PmThrowExceptionFmt( PM_HERE, PM_ERROR_SYSTEM, "can not get interface[0x%x]",IID_IPmUnknown); \
		return ptrUnknown;                                                                           \
	}
/**	\brief PMCOM_REG_CLASS_BEGIN
*/
#define PMCOM_REG_CLASS_BEGIN static PM_BOOL PmDllExportUtilCall( int nReg, const char* pszClassName, IPmUnknown** ppUnknown, PM_DWORD dwContext) {
/**	\brief PMCOM_REG_CLASS_BIND
*/
#define PMCOM_REG_CLASS_BIND( classtype,classname,ver ) \
	if( nReg == 1 ) { if( !pmcom_register_class( CLSID_##classname, ver, pmcom_get_dll_path() ) ) return PM_FALSE; }\
	if( nReg == 2 ) { pmcom_unregister_class( CLSID_##classname, ver ); }\
	if( nReg == 3 ) { if( strcmp( CLSID_##classname, pszClassName ) == 0 ) {\
		if( ppUnknown == PM_NULL ) return PM_FALSE;\
		classtype* pNewClassInstance = new classtype;\
		return pNewClassInstance->queryInterface( IID_IPmUnknown,(void**)ppUnknown );}}

#define PMCOM_REG_CLASS_BIND_PARA( classtype,classname,ver ) \
	if( nReg == 1 ) { if( !pmcom_register_class( CLSID_##classname, ver, pmcom_get_dll_path() ) ) return PM_FALSE; }\
	if( nReg == 2 ) { pmcom_unregister_class( CLSID_##classname, ver ); }\
	if( nReg == 3 ) { if( strcmp( CLSID_##classname, pszClassName ) == 0 ) {\
		if( ppUnknown == PM_NULL ) return PM_FALSE;\
		classtype* pNewClassInstance = new classtype( dwContext );\
		return pNewClassInstance->queryInterface( IID_IPmUnknown,(void**)ppUnknown );}}

/**	\brief PMCOM_REG_CLASS_END
*/	
#define PMCOM_REG_CLASS_END return PM_TRUE;}\
	extern "C" PM_BOOL PmComRegisterServer(){  return PmDllExportUtilCall( 1, PM_NULL, PM_NULL,0 ); }\
	extern "C" PM_BOOL PmComUnregisterServer(){  return PmDllExportUtilCall( 2, PM_NULL, PM_NULL,0 ); }\
	extern "C" PM_BOOL PmComCreateObject(PM_LPCSTR pszObjName, IPmUnknown**ppUnknown, PM_DWORD dwContext)\
		{  return PmDllExportUtilCall( 3, pszObjName,ppUnknown, dwContext); }

#endif

