#ifndef __PM_SDK_H__
#define __PM_SDK_H__


/**	
 *@file pm_sdk.h
 *@brief 
*/

#ifdef __cplusplus
extern "C"
{
#endif
//////////////////////////////////////////////////////////////////////////

/**	\brief	The pm_memcpy function

	\param	pDest	a parameter of type PM_LPVOID
	\param	pSrc	a parameter of type PM_LPCVOID
	\param	uiCount	a parameter of type PM_UINT

	\return	PM_LPVOID

	
*/
PM_LPVOID	pm_memcpy(	PM_LPVOID pDest,	PM_LPCVOID pSrc,	PM_UINT uiCount );
/**	\brief	The pm_memmove function

	\param	pDest	a parameter of type PM_LPVOID
	\param	pSrc	a parameter of type PM_LPCVOID
	\param	uiCount	a parameter of type PM_UINT

	\return	PM_LPVOID

	
*/

PM_LPVOID	pm_memmove(	PM_LPVOID pDest,	PM_LPCVOID pSrc,	PM_UINT uiCount );
/**	\brief	The pm_memset function

	\param	pDest	a parameter of type PM_LPVOID
	\param	iValue	a parameter of type PM_INT
	\param	uiCount	a parameter of type PM_UINT

	\return	PM_LPVOID

	
*/
PM_LPVOID	pm_memset(	PM_LPVOID pDest,	PM_INT iValue,		PM_UINT uiCount );
/**	\brief	The pm_memcmp function

	\param	pBuf1	a parameter of type PM_LPCVOID
	\param	pBuf2	a parameter of type PM_LPCVOID
	\param	uiCount	a parameter of type PM_UINT

	\return	PM_INT

	
*/
PM_INT		pm_memcmp(	PM_LPCVOID pBuf1,	PM_LPCVOID pBuf2,	PM_UINT uiCount);
/**	\brief	The pm_memicmp function

	\param	pBuf1	a parameter of type PM_LPCVOID
	\param	pBuf2	a parameter of type PM_LPCVOID
	\param	uiCount	a parameter of type PM_UINT

	\return	PM_INT

	
*/
PM_INT		pm_memicmp(	PM_LPCVOID pBuf1,	PM_LPCVOID pBuf2,	PM_UINT uiCount);

//////////////////////////////////////////////////////////////////////////
/**	\brief	The pm_malloc function

	\param	uiSize	a parameter of type PM_UINT

	\return	PM_LPVOID

	
*/
PM_LPVOID	pm_malloc(	PM_UINT uiSize	);
/**	\brief	The pm_free function

	\param	pData	a parameter of type PM_LPVOID

	\return	PM_VOID

	
*/
PM_VOID		pm_free(	PM_LPVOID pData	);

//////////////////////////////////////////////////////////////////////////
/**	\brief	The pm_strcmp function

	\param	pszcStr1	a parameter of type PM_LPCSTR  
	\param	pszcStr2	a parameter of type PM_LPCSTR

	\return	PM_INT

	
*/
PM_INT		pm_strcmp(	PM_LPCSTR	pszcStr1,	PM_LPCSTR pszcStr2 );
/**	\brief	The pm_strcat function

	\param	pszDest	a parameter of type PM_LPSTR   
	\param	pszcSource	a parameter of type PM_LPCSTR

	\return	PM_LPSTR

	
*/
PM_LPSTR	pm_strcat(	PM_LPSTR	pszDest,	PM_LPCSTR pszcSource );
/**	\brief	The pm_strstr function

	\param	pszcSource	a parameter of type PM_LPCSTR  
	\param	pszCharSet	a parameter of type PM_LPCSTR

	\return	PM_LPSTR

	
*/
PM_LPSTR	pm_strstr(	PM_LPCSTR	pszcSource,	PM_LPCSTR pszCharSet );
PM_LPSTR	pm_stristr(	PM_LPCSTR	pszcSource,	PM_LPCSTR pszCharSet );
PM_LPSTR	pm_strcasestr(	PM_LPCSTR	pszcSource,	PM_LPCSTR pszCharSet );

/**	\brief	The pm_stricmp function

	\param	pszcStr1	a parameter of type PM_LPCSTR  
	\param	pszcStr2	a parameter of type PM_LPCSTR

	\return	PM_INT

	
*/
PM_INT		pm_stricmp(	PM_LPCSTR	pszcStr1,	PM_LPCSTR pszcStr2 );
/**	\brief	The pm_strcasecmp function

	\param	pszcStr1	a parameter of type PM_LPCSTR  
	\param	pszcStr2	a parameter of type PM_LPCSTR

	\return	PM_INT

	
*/
PM_INT		pm_strcasecmp(	PM_LPCSTR	pszcStr1,	PM_LPCSTR pszcStr2 );
/**	\brief	The pm_strncpy function

	\param	pszDest	a parameter of type PM_LPSTR   
	\param	pzscSource	a parameter of type PM_LPCSTR
	\param	uiCount	a parameter of type PM_UINT

	\return	PM_LPSTR

	
*/
PM_LPSTR	pm_strncpy(	PM_LPSTR	pszDest,	PM_LPCSTR pzscSource,	PM_UINT uiCount );
/**	\brief	The pm_strncmp function

	\param	pszcStr1	a parameter of type PM_LPCSTR  
	\param	pszcStr2	a parameter of type PM_LPCSTR
	\param	uiCount	a parameter of type PM_UINT

	\return	PM_INT

	
*/
PM_INT		pm_strncmp(	PM_LPCSTR	pszcStr1,	PM_LPCSTR pszcStr2,		PM_UINT uiCount );
/**	\brief	The pm_strncat function

	\param	pszDest	a parameter of type PM_LPSTR   
	\param	pszcSource	a parameter of type PM_LPCSTR
	\param	uiCount	a parameter of type PM_UINT

	\return	PM_LPSTR

	
*/
PM_LPSTR	pm_strncat(	PM_LPSTR	pszDest,	PM_LPCSTR pszcSource,	PM_UINT uiCount );
/**	\brief	The pm_strlwr function

	\param	pszSource	a parameter of type PM_LPSTR   

	\return	PM_LPSTR

	
*/
PM_LPSTR	pm_strlwr(	PM_LPSTR	pszSource );
/**	\brief	The pm_strchr function

	\param	pszSource	a parameter of type PM_LPCSTR  
	\param	iValue	a parameter of type PM_INT

	\return	PM_LPSTR

	
*/
PM_LPSTR	pm_strchr(	PM_LPCSTR	pszSource,	PM_INT iValue );
/**	\brief	The pm_strlen function

	\param	pszSource	a parameter of type PM_LPCSTR  

	\return	PM_UINT

	
*/
PM_UINT		pm_strlen(	PM_LPCSTR	pszSource );
/**	\brief	The pm_strcpy function

	\param	pszDest	a parameter of type PM_LPSTR   
	\param	pszcSource	a parameter of type PM_LPCSTR

	\return	PM_LPSTR

	
*/
PM_LPSTR	pm_strcpy(	PM_LPSTR	pszDest,	PM_LPCSTR pszcSource );
/**	\brief	The pm_strrchr function

	\param	pszcSource	a parameter of type PM_LPCSTR  
	\param	iValue	a parameter of type PM_INT

	\return	PM_LPSTR

	
*/
PM_LPSTR	pm_strrchr(	PM_LPCSTR	pszcSource,	PM_INT iValue);
/**	\brief	The pm_strtok function

	\param	pszToken	a parameter of type PM_LPSTR   
	\param	pszcDelimit	a parameter of type PM_LPCSTR

	\return	PM_LPSTR

	
*/
PM_LPSTR	pm_strtok(	PM_LPSTR	pszToken,	PM_LPCSTR pszcDelimit );
/**	\brief	The pm_strupr function

	\param	pszSource	a parameter of type PM_LPSTR   

	\return	PM_LPSTR

	
*/
PM_LPSTR	pm_strupr(	PM_LPSTR	pszSource );

/**	\brief	The pm_strncasecmp function

	\param	psz1	a parameter of type PM_LPCSTR
	\param	psz2	a parameter of type PM_LPCSTR
	\param	n	a parameter of type PM_INT

	\return	PM_INT

	
*/
PM_INT pm_strncasecmp( PM_LPCSTR psz1, PM_LPCSTR psz2, PM_INT n );

/**	\brief	The pm_sprintf function 

	\param	*	a parameter of type char
	\param	*	a parameter of type const char

	\return	int

	
*/
int pm_sprintf(char *, const char *, ...);
/**	\brief	The pm_vsprintf function

	\param	*	a parameter of type char
	\param	*	a parameter of type const char

	\return	int

	
*/
int pm_vsprintf(char *, const char *, PM_ARGLIST );

/**	\brief	The pm_printf function

	\param	*	a parameter of type const char

	\return	int

	
*/
int pm_printf(const char *, ...);
/**	\brief	The pm_vprintf function

	\param	*	a parameter of type const char

	\return	int

	
*/
int pm_vprintf(const char *, PM_ARGLIST );


/**	\brief	The pm_lower function

	\param	chSrc	a parameter of type char

	\return	char

	
*/
char pm_lower( char chSrc );
/**	\brief	The pm_upper function

	\param	chSrc	a parameter of type char

	\return	char

	
*/
char pm_upper( char chSrc );

//////////////////////////////////////////////////////////////////////////
//PM_INT pm_getchar( PM_VOID );
//不提供系统的format函数，必须使用pm_xxx自己提供的

/**	\brief	The pm_exit function

	\param	n	a parameter of type int

	\return	void

	
*/
void pm_exit(int n);
/**	\brief	The pm_abs function

	\param	n	a parameter of type int

	\return	int

	
*/
int pm_abs( int n );
/**	\brief	The pm_atol function

	\param	psz	a parameter of type const char *

	\return	long

	
*/
long   pm_atol(const char *psz);
/**	\brief	The pm_atoi function

	\param	psz	a parameter of type const char *

	\return	int

	
*/
int    pm_atoi(const char *psz);
/**	\brief	The pm_atof function

	\param	psz	a parameter of type const char *

	\return	double

	
*/
double pm_atof(const char *psz);
/**	\brief	The pm_rand function


	\return	int

	
*/
int    pm_rand();
/**	\brief	The pm_srand function

	\param	n	a parameter of type unsigned int

	\return	void

	
*/
void   pm_srand(unsigned int n);
/**	\brief	The pm_strtod function

	\param	psz	a parameter of type const char *
	\param	*ppend	a parameter of type char *

	\return	double

	
*/
double pm_strtod(const char *psz, char **ppend);
/**	\brief	The pm_strtol function

	\param	psz	a parameter of type const char *
	\param	*ppend	a parameter of type char *
	\param	base	a parameter of type int

	\return	long

	
*/
long   pm_strtol(const char *psz, char **ppend, int base);
/**	\brief	The pm_v_system function substitutes pm_system
		while the parent process has taken huge memory.

	\param	psz	a parameter of type const char *

	\return	int
	

*/
int    pm_v_system(const char *psz, int timeout);
/**	\brief	The pm_system function

	\param	psz	a parameter of type const char *

	\return	int

	
*/
int    pm_system(const char *psz);
/**	\brief	The pm_ecvt function

	\param	d	a parameter of type double
	\param	i	a parameter of type int
	\param	dec	a parameter of type int *
	\param	sign	a parameter of type int *

	\return	char *

	
*/
char * pm_ecvt(double d, int i, int *dec, int *sign);
/**	\brief	The pm_fcvt function

	\param	d	a parameter of type double
	\param	i	a parameter of type int
	\param	dec	a parameter of type int *
	\param	sign	a parameter of type int *

	\return	char *

	
*/
char * pm_fcvt(double d, int i, int *dec, int *sign);
/**	\brief	The pm_gcvt function

	\param	d	a parameter of type double
	\param	i	a parameter of type int
	\param	buf	a parameter of type char *

	\return	char *

	
*/
char * pm_gcvt(double d, int i, char *buf);

/**	\brief	The pm_sleep function

	\param	dwMilliseconds	a parameter of type PM_DWORD

	\return	void

	
*/
void pm_sleep( PM_DWORD dwMilliseconds );
/**	\brief	The pm_getenv function

	\param	varname	a parameter of type const char *

	\return	char*

	
*/
char* pm_getenv( const char *varname );
/**	\brief	The pm_getpid function


	\return	PM_DWORD

	
*/
PM_DWORD pm_getpid();
/**	\brief	The pm_gettid function
			获得当前线程id
	\return	PM_DWORD
*/
PM_DWORD pm_gettid();

#ifdef WIN32
#define PM_PROCESS_SYSTEM_ROOT_ID	0
#else
#define PM_PROCESS_SYSTEM_ROOT_ID	1
#endif

/**	\brief	The pm_getppid function
			获得父进程ID
	\return	PM_DWORD
*/
PM_DWORD pm_getppid();

const char* pm_getpwd();
const char* pm_get_exe_name();

PM_INT64 pm_atoll( const char* psz );

#ifdef __cplusplus
}
#endif

#endif
