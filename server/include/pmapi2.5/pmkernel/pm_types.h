#ifndef __PM__TYPES__
#define __PM__TYPES__
/**
 *@file pm_types.h
 *@brief PM的简单数据类型定义
 */

/**	\brief	PM_TRUE
*/
#define PM_TRUE	(1)
/**	\brief	PM_FALSE
*/
#define PM_FALSE (0)


#ifdef __cplusplus
/**	\brief	PM_NULL

*/
	#define PM_NULL    (0L)
/**	\brief	PM_SET_DEFAULT

*/
	#define PM_SET_DEFAULT( defValue ) = (defValue)
#else
/**	\brief	PM_NULL

*/
	#define PM_NULL    ((void *)0)
/**	\brief	PM_SET_DEFAULT

*/
	#define PM_SET_DEFAULT( defValue )
#endif
/**	\brief	PM_MAX_PATH

*/
#define PM_MAX_PATH 260

/**	\brief	Redefinition of long            as PM_LONG
*/
typedef long			PM_LONG;
/**	\brief	Redefinition of unsigned long   as PM_ULONG
*/
typedef unsigned long	PM_ULONG;
/**	\brief	Redefinition of int             as PM_INT
*/
typedef int				PM_INT;
/**	\brief	Redefinition of unsigned int    as PM_UINT
*/
typedef unsigned int	PM_UINT;
/**	\brief	Redefinition of short           as PM_SHORT
*/
typedef short			PM_SHORT;
/**	\brief	Redefinition of unsigned short  as PM_USHORT
*/
typedef unsigned short	PM_USHORT;
/**	\brief	Redefinition of char            as PM_CHAR
*/
typedef char			PM_CHAR;
/**	\brief	Redefinition of unsigned char   as PM_UCHAR
*/
typedef unsigned char	PM_UCHAR;


/**	\brief	Redefinition of int             as PM_BOOL
*/
typedef int				PM_BOOL;
/**	\brief	Redefinition of unsigned char   as PM_BYTE
*/
typedef unsigned char	PM_BYTE;
/**	\brief	Redefinition of unsigned short  as PM_WORD
*/
typedef unsigned short	PM_WORD;
/**	\brief	Redefinition of unsigned long   as PM_DWORD
*/
typedef unsigned long	PM_DWORD;
/**	\brief	Redefinition of double          as PM_DOUBLE
*/
typedef double			PM_DOUBLE;

/**	\brief	Redefinition of char *          as PM_LPSTR
*/
typedef char *			PM_LPSTR;
/**	\brief	Redefinition of const char *    as PM_LPCSTR
*/
typedef const char *	PM_LPCSTR;

/**	\brief	Redefinition of void            as PM_VOID
*/
typedef void 			PM_VOID;
/**	\brief	Redefinition of void *          as PM_LPVOID
*/
typedef void *			PM_LPVOID;
/**	\brief	Redefinition of const void *    as PM_LPCVOID
*/
typedef const void *	PM_LPCVOID;
/**	\brief	Redefinition of void *          as PM_HANDLE
*/
typedef void *			PM_HANDLE;
/**	\brief	Redefinition of PM_DWORD        as PM_HRESULT
*/
typedef PM_DWORD		PM_HRESULT;
#ifdef WIN32
/**	\brief	Redefinition of unsigned int as PM_SOCKET
*/
	typedef unsigned int PM_SOCKET;
#else
/**	\brief	Redefinition of int         as PM_SOCKET
*/
	typedef int			PM_SOCKET;
#endif
/**	\brief	Redefinition of PM_DWORD        as PM_IPADDR_V4
*/
typedef PM_DWORD		PM_IPADDR_V4;
/**	\brief	Redefinition of PM_USHORT       as PM_PORT
*/
typedef PM_USHORT		PM_PORT;

/**	\brief	PM_INVALID_SOCKET

*/
#define PM_INVALID_SOCKET ((PM_SOCKET)-1)
/**	\brief	PMSOCK_ERROR

*/
#define PMSOCK_ERROR	(-1)
/**	\brief	PMSOCK_INFINITE

*/
#define PMSOCK_INFINITE 0x7fffffff

#ifdef WIN32
/**	\brief	Redefinition of __int64         as PM_INT64
*/
typedef __int64			PM_INT64;
#else
/**	\brief	Redefinition of long long       as PM_INT64
*/
typedef long long		PM_INT64;
#endif

/**	\brief	PM_ERROR

*/
#define PM_ERROR (-1)
/**	\brief	PM_OK

*/
#define PM_OK (0)
/**	\brief	PM_INVALID_HANDLE_VALUE

*/
#define PM_INVALID_HANDLE_VALUE ((PM_HANDLE)(-1))
/**	\brief	PM_MAX

*/
#define PM_MAX(x, y) ((x) > (y) ? (x) : (y))
/**	\brief	PM_MIN

*/
#define PM_MIN(x, y) ((x) < (y) ? (x) : (y))
/**	\brief	PM_DIGIT

*/
#define PM_DIGIT(ch) ((ch) >= '0' && (ch) <= '9')
///16进制
/**	\brief	PM_HEX

*/
#define PM_HEX(ch) (((ch) >= '0' && (ch) <= '9')||((ch) >= 'a' && (ch) <= 'f')||((ch) >= 'A' && (ch) <= 'F'))
/**	\brief	PM_LOWALPHA

*/
#define PM_LOWALPHA(ch) ((ch) >= 'a' && (ch) <= 'z' ) //小写字母
/**	\brief	PM_HIGHALPHA

*/
#define PM_HIGHALPHA(ch) ((ch) >= 'A' && (ch) <= 'Z')//大写写字母
/**	\brief	PM_ALPHA

*/
#define PM_ALPHA(ch) (PM_LOWALPHA(ch) || PM_HIGHALPHA(ch))//所有字母

#define PM_INFINITE (-1)

//typedef enum pm_program_status
//{
//	PMPS_RUNNING,
//	PMPS_DEAD
// }PM_PROGRAM_STATUS;

/**	\brief	The tagByteStrRegiterInfo struct

*/
// typedef struct tagByteStrRegiterInfo
// {
// 	void* pThis;
// 	char** ppText;
// 	void* pData;
// }ByteStrRegiterInfo;

/**	\brief	The pm_bytestr struct

*/
//typedef struct pm_bytestr ByteStr;
#endif

