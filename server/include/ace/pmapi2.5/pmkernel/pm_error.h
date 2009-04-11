/**
 *@file pm_error.h
 *@brief error define
*/

#ifndef __PM__ERROR__
#define __PM__ERROR__


#ifdef __cplusplus
extern "C"
{
#endif
/** \brief pm_set_error

*/
void pm_set_error( PM_DWORD dwError );
/** \brief pm_get_error

*/
PM_DWORD pm_get_error();
/** \brief pm_get_system_error

*/
PM_DWORD pm_get_system_error();
#define pm_errno pm_get_system_error()

char* pm_strerror( PM_DWORD err, char* buf, int len );

#ifdef __cplusplus
}
#endif

/** \brief PM_ERROR_UNKNOWN

*/
#define PM_ERROR_UNKNOWN					0xffffffff
/** \brief PM_ERROR_SYSTEM

*/
#define PM_ERROR_SYSTEM						1
/** \brief PM_ERROR_BASE64DE

*/
#define PM_ERROR_BASE64DE					2
/** \brief PM_MEMORY_NOT_ENOUGH

*/
#define PM_MEMORY_NOT_ENOUGH				3		

//注册接口已经存在
/** \brief PM_REGISTER_INTERFACE_ALREADYEXIST

*/
#define PM_REGISTER_INTERFACE_ALREADYEXIST	4
//名称格式不正确	
/** \brief PM_INTERFACE_FORMAT_NOT_RIGHT

*/
#define PM_INTERFACE_FORMAT_NOT_RIGHT		5
//未发现该接口			
/** \brief PM_INTERFACE_NOT_FOUND

*/
#define PM_INTERFACE_NOT_FOUND				6
/** \brief PM_INTERFACE_CREATE_FAIL

*/
#define PM_INTERFACE_CREATE_FAIL			7
//字符串没有找到
/** \brief PM_STRING_NOT_FOUND

*/
#define PM_STRING_NOT_FOUND					8
/** \brief PM_ERROR_NOTSUPPORT

*/
#define PM_ERROR_NOTSUPPORT					9
/** \brief PM_ERROR_CEF_CORE

*/
#define PM_ERROR_CEF_CORE					10
/** \brief PM_FATAL_CEF_CORE

*/
#define PM_FATAL_CEF_CORE					11
/** \brief PM_ERROR_CEF_ADDIN

*/
#define PM_ERROR_CEF_ADDIN					12
/** \brief PM_FATAL_CEF_ADDIN

*/
#define PM_FATAL_CEF_ADDIN					13
/** \brief PM_ERROR_CEF_PROTO

*/
#define PM_ERROR_CEF_PROTO					14
/** \brief PM_FATAL_CEF_PROTO

*/
#define PM_FATAL_CEF_PROTO					15
/** \brief PM_ERROR_CEF_SDK

*/
#define PM_ERROR_CEF_SDK					16
/** \brief PM_FATAL_CEF_SDK

*/
#define PM_FATAL_CEF_SDK					17
/** \brief PM_ERROR_DB_CONNECT

*/
#define PM_ERROR_DB_CONNECT					18
/** \brief PM_ERROR_DB_QUERY

*/
#define PM_ERROR_DB_QUERY					19
/** \brief PM_ERROR_DB_EXECUTE

*/
#define PM_ERROR_DB_EXECUTE					20
/** \brief PM_ERROR_DB_STORE_RESULT

*/
#define PM_ERROR_DB_STORE_RESULT			21
/** \brief PM_ERROR_DB_NODATA_OBSOLETE

*/
#define PM_ERROR_DB_NODATA_OBSOLETE			22
/** \brief PM_ERROR_THREAD

*/
#define PM_ERROR_THREAD						23
/** \brief PM_ERROR_SOCKET

*/
#define PM_ERROR_SOCKET						24
/** \brief PM_ERROR_CMPROTOCOL

*/
#define PM_ERROR_CMPROTOCOL					25
/** \brief PM_ERROR_NODATA

*/
#define PM_ERROR_NODATA						26
#define PM_ERROR_PROPERTY_NOTEXIST			27

#define PM_ERROR_CRASH_REPEATED				28
#define PM_ERROR_TIMEOUT 29

/*1000 - 1999错误号分配于cef*/

/*2000 - 2999错误号分配于产品插件POP3/MAPI/LOTUS*/

/*

#define PM_ERROR_IPC_OPEN			15
#define PM_ERROR_QUEUE_CONN			16				// failed to connect to queue manager
#define PM_ERROR_QUEUE_PUTDATA 17				// failed to put data in queue
#define PM_ERROR_QUEUE_GETDATA 18				// failed to get data
#define PM_ERROR_QUEUE_REMOVEDATA 19			// failed to remove data
#define PM_ERROR_QUEUE_FINDDATA 20			// can't find specific data
#define PM_ERROR_NODATA 21					// no data available in queue
#define PM_ERROR_QUEUE_URI 22					// queue uri can't be parsed
#define PM_ERROR_QUEUE_OPEN 23				// failed to open queue
#define PM_ERROR_LOSTCONNECTION 24			// lost connection to queue
#define PM_ERROR_GMQC_CONNECT 25				// IGMQConn object can't connect to queue server
#define PM_ERROR_POP3_UIDL 27
#define PM_ERROR_PARAMETER_PTR_NULL 28		//参数指针为空


*/



#endif
