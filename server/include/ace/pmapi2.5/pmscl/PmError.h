#ifndef __PM_SCL_ERROR_H__
#define __PM_SCL_ERROR_H__

class PmByteStream;
typedef PmByteStream PmString;

/**	
 *@file PmError.h 
 *@brief 
*/

#ifdef WIN32
const char* PmGetSourceFileName( const char* pszFileName );

#define PM_HERE "", PmGetSourceFileName(__FILE__), __LINE__
#else
/** \brief PM_HERE
*/
#define PM_HERE __FUNCTION__, __FILE__, __LINE__
#endif

/**	\brief	The PmThrowException function

	\param	pszFunctionName	a parameter of type const char*
	\param	pszSrcFileName	a parameter of type const char*
	\param	nSrcFileLine	a parameter of type int
	\param	dwErrorNo	a parameter of type PM_DWORD
	\param	pszFormat	a parameter of type const char*

	\return	void

	
*/
void PmThrowExceptionFmt( const char* pszFunctionName, const char* pszSrcFileName, int nSrcFileLine, 
					  PM_DWORD dwErrorNo, const char* pszFormat, ... );

void PmThrowException( const char* pszFunctionName, const char* pszSrcFileName, int nSrcFileLine, 
					  PM_DWORD dwErrorNo, const char* pszInfo );


#define pm_throw pm_throw0

#define pm_throw0( s ) PmThrowException( PM_HERE, PM_ERROR_UNKNOWN, s )
#define pm_throw1( s, p1 ) PmThrowExceptionFmt( PM_HERE, PM_ERROR_UNKNOWN, s, p1 )
#define pm_throw2( s, p1,p2 ) PmThrowExceptionFmt( PM_HERE, PM_ERROR_UNKNOWN, s, p1,p2 )
#define pm_throw3( s, p1,p2,p3 ) PmThrowExceptionFmt( PM_HERE, PM_ERROR_UNKNOWN, s, p1,p2,p3 )
#define pm_throw4( s, p1,p2,p3,p4 ) PmThrowExceptionFmt( PM_HERE, PM_ERROR_UNKNOWN, s, p1,p2,p3,p4 )
#define pm_throw5( s, p1,p2,p3,p4,p5 ) PmThrowExceptionFmt( PM_HERE, PM_ERROR_UNKNOWN, s, p1,p2,p3,p4,p5 )


/**	\brief	The PmGetSystemErrorText function

	\param	dwErrorNo	a parameter of type PM_DWORD

	\return	PmString

	
*/
PmString PmGetSystemErrorText( PM_DWORD dwErrorNo = pm_get_system_error() );
#define _PM_ERRSTR (PmGetSystemErrorText().c_str())

/**	\brief PM_CATCH_FLAG
*/
#define PM_CATCH_FLAG function_must_be_try_cached_ask_liubin_for_any_question
/**	\brief PM_TRY
*/
#define PM_TRY \
try { \
 int PM_CATCH_FLAG = 1; \
/**	\brief PM_CATCH
*/
#define PM_CATCH \
	} catch

/**	\brief PM_THROWABLE_METHOD
*/
#define PM_THROWABLE_METHOD(func, args)                                                                  \
			func args;                                                                                   \
do {                                                                                                     \
	PM_CATCH_FLAG = 0;                                                                                   \
}while(0)
/**	\brief PM_CALL_THROWABLE_FUNCTION
*/
#define PM_CALL_THROWABLE_FUNCTION(func_call) \
do { \
	int PM_CATCH_FLAG = 0; \
	func_call; \
}while(0)
/**	\brief PM_CALL_THROWABLE_METHOD
*/
#define PM_CALL_THROWABLE_METHOD(object, func_call) \
do { \
	int PM_CATCH_FLAG = 0; \
	try {                                                                                                \
		(object).func_call; \
	}                                                                                                \
	catch(PmException &ex) {                                                                             \
			ex->append(PM_HERE, PM_ERROR_UNKNOWN, "function is throwed by calling PM_THROWABLE_METHOD"); \
			throw ex;                                                                                    \
		}                                                                                                \
}while(0)

#endif
