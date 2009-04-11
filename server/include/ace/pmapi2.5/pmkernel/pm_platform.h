#ifndef __PM_PLATFORM_H__
#define __PM_PLATFORM_H__

/**	
 *@file pm_platform.h
 *@brief 
*/

#ifdef __cplusplus
extern "C"{
#endif

#ifdef WIN32
	#ifdef _DEBUG
		#define PM_DEBUG
	#endif
#else
	#ifdef DEBUG
		#define PM_DEBUG
	#endif
#endif

/** \brief PM_LITTLEENDIAN

*/
#define PM_LITTLEENDIAN//Ä¬ÈÏLITTLEENDIAN

/** \brief PM_ARGLIST

*/
#ifdef WIN32
	typedef char* PM_ARGLIST;
#else
	//yag
	//typedef void* PM_ARGLIST;
	#include <stdarg.h>
	typedef va_list PM_ARGLIST;
#endif

#ifdef PM_SUN_OS//sun os BIGENDIAN
	#undef PM_LITTLEENDIAN
	#define PM_BIGENDIAN
#endif

#ifdef WIN32
/** \brief PM_CRLF

*/
	#define PM_CRLF "\r\n"
/** \brief PM_CRLF_LEN

*/
	#define PM_CRLF_LEN 2
/** \brief PM_SLASH

*/
	#define PM_SLASH "\\"
/** \brief PM_SLASH_CHAR

*/
	#define PM_SLASH_CHAR '\\'
#else
/** \brief PM_CRLF

*/
	#define PM_CRLF "\n"
/** \brief PM_CRLF_LEN

*/
	#define PM_CRLF_LEN 1
/** \brief PM_SLASH

*/
	#define PM_SLASH "/"
/** \brief PM_SLASH_CHAR

*/
	#define PM_SLASH_CHAR '/'
#endif

#ifdef WIN32
	PM_HANDLE pm_get_lowest_sa();
#endif


#ifdef __cplusplus
}
#endif

#endif

