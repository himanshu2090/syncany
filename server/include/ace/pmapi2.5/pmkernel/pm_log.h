#ifndef __pm_log_H__
#define __pm_log_H__

#include <time.h>
/**	
 *@file pm_log.h
 *@brief 
*/

/** \brief pm_log_level_fatal

*/
#define pm_log_level_fatal		0
/** \brief pm_log_level_error

*/
#define pm_log_level_error 		1
/** \brief pm_log_level_warning

*/
#define pm_log_level_warning	2
/** \brief pm_log_level_info

*/
#define pm_log_level_info 		3
/** \brief pm_log_level_success

*/
#define pm_log_level_success 	4
/** \brief pm_log_level_log

*/
#define pm_log_level_log		5
/** \brief pm_log_level_debug

*/
#define pm_log_level_debug 		6
/** \brief pm_log_TRACE

*/
#define pm_log_level_trace		7
/** \brief pm_log_level_dump

*/
#define pm_log_level_dump		8

extern PM_INT g_bPrintfOnScreen;
extern PM_INT g_bRecordSourceInfo;
extern PM_INT g_iLogLevel;

#ifdef __cplusplus
extern "C"
{
#endif

/// 为了兼容记录为 mig1000万 日志形式而添加 cheng_qi_peng (NOT USE)
//typedef void (*WIRETE_MIG1000_LOG_FUNC)(PM_INT, PM_LPCSTR, PM_INT, PM_LPCSTR, va_list);
//extern void pm_set_wrt_mig1000_logfunc(WIRETE_MIG1000_LOG_FUNC);

/**
 *@brief 以附加方式打开已经存在的文件，或创建一个新文件
 *@param[in]  pszcLogname 文件名称
 *@return 0:成功；1:失败
 */
PM_INT pm_log_open ( PM_LPCSTR pszcLogname);


/**	
 *\brief	The pm_log_daily_open 设置日志文件目录和名称前缀
 *\param[in]	pszcDirName	目录名
 *\param[in]	pszcModule	名称前缀 
 *\return	0:成功；1:失败	
 */
PM_INT pm_log_daily_open(PM_LPCSTR pszcDirName, PM_LPCSTR pszcModule);

///pm_log_close 关闭文件
PM_VOID pm_log_close();

/**	
 *\brief	The pm_log_print_on_screen 设置日志内容是否打印到屏幕
 *\param[in]	iVal	设置的值，0:不打印；其它则打印	
 */
PM_VOID pm_log_print_on_screen(PM_INT iVal);

/**	
 *\brief	The pm_log_set_level 设置日志输出的级别
 *\param[in]	iLevel	设置的值(0---9)
 */
PM_VOID pm_log_set_level(PM_INT iLevel);


/**	\brief	pm_log_wrap_fmt 日志文件格式的封装
	\param	iLevel	日志级别
	\param	pszcSrcFile	源文件
	\param	iSrcLine	行号
	\param	pszcFmt	字符串格式	
*/
PM_VOID pm_log_wrap_fmt(PM_INT iLevel, PM_LPCSTR pszcSrcFile, PM_INT iSrcLine, PM_LPCSTR pszcFmt,...);
PM_VOID pm_log_wrap(PM_INT iLevel, PM_LPCSTR pszcSrcFile, PM_INT iSrcLine, PM_LPCSTR pszInfo );

/**	\brief	pm_log_dump 日志内容dump输出
	\param	lpData	要输出的内容
	\param	iLen	输出的数据长度
	\param	pszcFormat	输出标题格式
*/
PM_VOID pm_dump_fmt( PM_LPVOID lpData, PM_INT iLen, PM_LPCSTR pszcFormat, ... );
PM_VOID pm_dump( PM_LPVOID lpData, PM_INT iLen, PM_LPCSTR pszInfo );
/** \brief pm_dump_real

*/
PM_VOID pm_dump_fmt_real( PM_LPVOID lpData, PM_INT iLen, PM_LPCSTR pszcFormat, ... );
PM_VOID pm_dump_real( PM_LPVOID lpData, PM_INT iLen, PM_LPCSTR pszInfo );
/** \brief pm_dump_buffer

*/
PM_VOID pm_dump_buffer( PM_LPVOID lpData, PM_INT iLen );

///在创建文件之前的回调, 可以做一些特殊的工作，比如调整文件的权限等	
typedef void(*pm_openfile_callback)( const char* psz );

///设置回调函数，但创建文件时调用，默认不调	 
PM_VOID pm_set_openfile_callback( pm_openfile_callback fnopenfilecallback ) ;


///各种级别的日志输出
PM_VOID pm_fatal_fmt( PM_LPCSTR pszcFormat, ... );
PM_VOID pm_error_fmt( PM_LPCSTR pszcFormat, ... );
PM_VOID pm_warning_fmt( PM_LPCSTR pszcFormat, ... );
PM_VOID pm_info_fmt(PM_LPCSTR pszcFormat, ...);
PM_VOID pm_success_fmt(PM_LPCSTR pszcFormat, ...);
PM_VOID pm_log_fmt( PM_LPCSTR pszcFormat, ... );
PM_VOID pm_debug_fmt( PM_LPCSTR pszcFormat, ... );
PM_VOID pm_trace_fmt( PM_LPCSTR pszcFormat, ... ); 

#define pm_fatal pm_fatal0
#define pm_error pm_error0
#define pm_warning pm_warning0
#define pm_info pm_info0
#define pm_success pm_success0
#define pm_log pm_log0
#define pm_debug pm_debug0
#define pm_trace pm_trace0

void pm_log_set_record_source_info( int );

/** \brief pm_fatal0

*/
#define pm_fatal0( fmt ) pm_log_wrap( pm_log_level_fatal,__FILE__,__LINE__,fmt )
/** \brief pm_fatal1

*/
#define pm_fatal1( fmt,PM_p1 ) pm_log_wrap_fmt( pm_log_level_fatal,__FILE__,__LINE__,fmt,PM_p1 )
/** \brief pm_fatal2

*/
#define pm_fatal2( fmt,PM_p1,PM_p2 ) pm_log_wrap_fmt( pm_log_level_fatal,__FILE__,__LINE__,fmt,PM_p1,PM_p2 )
/** \brief pm_fatal3

*/
#define pm_fatal3( fmt,PM_p1,PM_p2,PM_p3 ) pm_log_wrap_fmt( pm_log_level_fatal,__FILE__,__LINE__,fmt,PM_p1,PM_p2,PM_p3 )
/** \brief pm_fatal4

*/
#define pm_fatal4( fmt,PM_p1,PM_p2,PM_p3,PM_p4 ) pm_log_wrap_fmt( pm_log_level_fatal,__FILE__,__LINE__,fmt,PM_p1,PM_p2,PM_p3,PM_p4 )
/** \brief pm_fatal5

*/
#define pm_fatal5( fmt,PM_p1,PM_p2,PM_p3,PM_p4,PM_p5 ) pm_log_wrap_fmt( pm_log_level_fatal,__FILE__,__LINE__,fmt,PM_p1,PM_p2,PM_p3,PM_p4,PM_p5 )
/** \brief pm_error0

*/
#define pm_error0( fmt ) pm_log_wrap( pm_log_level_error,__FILE__,__LINE__,fmt )
/** \brief pm_error1

*/
#define pm_error1( fmt,PM_p1 ) pm_log_wrap_fmt( pm_log_level_error,__FILE__,__LINE__,fmt,PM_p1 )
/** \brief pm_error2

*/
#define pm_error2( fmt,PM_p1,PM_p2 ) pm_log_wrap_fmt( pm_log_level_error,__FILE__,__LINE__,fmt,PM_p1,PM_p2 )
/** \brief pm_error3

*/
#define pm_error3( fmt,PM_p1,PM_p2,PM_p3 ) pm_log_wrap_fmt( pm_log_level_error,__FILE__,__LINE__,fmt,PM_p1,PM_p2,PM_p3 )
/** \brief pm_error4

*/
#define pm_error4( fmt,PM_p1,PM_p2,PM_p3,PM_p4 ) pm_log_wrap_fmt( pm_log_level_error,__FILE__,__LINE__,fmt,PM_p1,PM_p2,PM_p3,PM_p4 )
/** \brief pm_error5

*/
#define pm_error5( fmt,PM_p1,PM_p2,PM_p3,PM_p4,PM_p5 ) pm_log_wrap_fmt( pm_log_level_error,__FILE__,__LINE__,fmt,PM_p1,PM_p2,PM_p3,PM_p4,PM_p5 )

/** \brief pm_warning0

*/
#define pm_warning0( fmt ) pm_log_wrap( pm_log_level_warning,__FILE__,__LINE__,fmt )
/** \brief pm_warning1

*/
#define pm_warning1( fmt,PM_p1 ) pm_log_wrap_fmt( pm_log_level_warning,__FILE__,__LINE__,fmt,PM_p1 )
/** \brief pm_warning2

*/
#define pm_warning2( fmt,PM_p1,PM_p2 ) pm_log_wrap_fmt( pm_log_level_warning,__FILE__,__LINE__,fmt,PM_p1,PM_p2 )
/** \brief pm_warning3

*/
#define pm_warning3( fmt,PM_p1,PM_p2,PM_p3 ) pm_log_wrap_fmt( pm_log_level_warning,__FILE__,__LINE__,fmt,PM_p1,PM_p2,PM_p3 )
/** \brief pm_warning4

*/
#define pm_warning4( fmt,PM_p1,PM_p2,PM_p3,PM_p4 ) pm_log_wrap_fmt( pm_log_level_warning,__FILE__,__LINE__,fmt,PM_p1,PM_p2,PM_p3,PM_p4 )
/** \brief pm_warning5

*/
#define pm_warning5( fmt,PM_p1,PM_p2,PM_p3,PM_p4,PM_p5 ) pm_log_wrap_fmt( pm_log_level_warning,__FILE__,__LINE__,fmt,PM_p1,PM_p2,PM_p3,PM_p4,PM_p5 )

/** \brief pm_info0

*/
#define pm_info0( fmt ) pm_log_wrap( pm_log_level_info,__FILE__,__LINE__,fmt )
/** \brief pm_info1

*/
#define pm_info1( fmt,PM_p1 ) pm_log_wrap_fmt( pm_log_level_info,__FILE__,__LINE__,fmt,PM_p1 )
/** \brief pm_info2

*/
#define pm_info2( fmt,PM_p1,PM_p2 ) pm_log_wrap_fmt( pm_log_level_info,__FILE__,__LINE__,fmt,PM_p1,PM_p2 )
/** \brief pm_info3

*/
#define pm_info3( fmt,PM_p1,PM_p2,PM_p3 ) pm_log_wrap_fmt( pm_log_level_info,__FILE__,__LINE__,fmt,PM_p1,PM_p2,PM_p3 )
/** \brief pm_info4

*/
#define pm_info4( fmt,PM_p1,PM_p2,PM_p3,PM_p4 ) pm_log_wrap_fmt( pm_log_level_info,__FILE__,__LINE__,fmt,PM_p1,PM_p2,PM_p3,PM_p4 )
/** \brief pm_info5

*/
#define pm_info5( fmt,PM_p1,PM_p2,PM_p3,PM_p4,PM_p5 ) pm_log_wrap_fmt( pm_log_level_info,__FILE__,__LINE__,fmt,PM_p1,PM_p2,PM_p3,PM_p4,PM_p5 )

/** \brief pm_success0

*/
#define pm_success0( fmt ) pm_log_wrap( pm_log_level_success,__FILE__,__LINE__,fmt )
/** \brief pm_success1

*/
#define pm_success1( fmt,PM_p1 ) pm_log_wrap_fmt( pm_log_level_success,__FILE__,__LINE__,fmt,PM_p1 )
/** \brief pm_success2

*/
#define pm_success2( fmt,PM_p1,PM_p2 ) pm_log_wrap_fmt( pm_log_level_success,__FILE__,__LINE__,fmt,PM_p1,PM_p2 )
/** \brief pm_success3

*/
#define pm_success3( fmt,PM_p1,PM_p2,PM_p3 ) pm_log_wrap_fmt( pm_log_level_success,__FILE__,__LINE__,fmt,PM_p1,PM_p2,PM_p3 )
/** \brief pm_success4

*/
#define pm_success4( fmt,PM_p1,PM_p2,PM_p3,PM_p4 ) pm_log_wrap_fmt( pm_log_level_success,__FILE__,__LINE__,fmt,PM_p1,PM_p2,PM_p3,PM_p4 )
/** \brief pm_success5

*/
#define pm_success5( fmt,PM_p1,PM_p2,PM_p3,PM_p4,PM_p5 ) pm_log_wrap_fmt( pm_log_level_success,__FILE__,__LINE__,fmt,PM_p1,PM_p2,PM_p3,PM_p4,PM_p5 )

/** \brief pm_log0

*/
#define pm_log0( fmt ) pm_log_wrap( pm_log_level_log,__FILE__,__LINE__,fmt )
/** \brief pm_log1

*/
#define pm_log1( fmt,PM_p1 ) pm_log_wrap_fmt( pm_log_level_log,__FILE__,__LINE__,fmt,PM_p1 )
/** \brief pm_log2

*/
#define pm_log2( fmt,PM_p1,PM_p2 ) pm_log_wrap_fmt( pm_log_level_log,__FILE__,__LINE__,fmt,PM_p1,PM_p2 )
/** \brief pm_log3

*/
#define pm_log3( fmt,PM_p1,PM_p2,PM_p3 ) pm_log_wrap_fmt( pm_log_level_log,__FILE__,__LINE__,fmt,PM_p1,PM_p2,PM_p3 )
/** \brief pm_log4

*/
#define pm_log4( fmt,PM_p1,PM_p2,PM_p3,PM_p4 ) pm_log_wrap_fmt( pm_log_level_log,__FILE__,__LINE__,fmt,PM_p1,PM_p2,PM_p3,PM_p4 )
/** \brief pm_log5

*/
#define pm_log5( fmt,PM_p1,PM_p2,PM_p3,PM_p4,PM_p5 ) pm_log_wrap_fmt( pm_log_level_log,__FILE__,__LINE__,fmt,PM_p1,PM_p2,PM_p3,PM_p4,PM_p5 )

/** \brief pm_assert0

*/
#define pm_assert0( fmt ) pm_log_wrap( pm_log_level_assert,__FILE__,__LINE__,fmt )
/** \brief pm_assert1

*/
#define pm_assert1( fmt,PM_p1 ) pm_log_wrap_fmt( pm_log_level_assert,__FILE__,__LINE__,fmt,PM_p1 )
/** \brief pm_assert2

*/
#define pm_assert2( fmt,PM_p1,PM_p2 ) pm_log_wrap_fmt( pm_log_level_assert,__FILE__,__LINE__,fmt,PM_p1,PM_p2 )
/** \brief pm_assert3

*/
#define pm_assert3( fmt,PM_p1,PM_p2,PM_p3 ) pm_log_wrap_fmt( pm_log_level_assert,__FILE__,__LINE__,fmt,PM_p1,PM_p2,PM_p3 )
/** \brief pm_assert4

*/
#define pm_assert4( fmt,PM_p1,PM_p2,PM_p3,PM_p4 ) pm_log_wrap_fmt( pm_log_level_assert,__FILE__,__LINE__,fmt,PM_p1,PM_p2,PM_p3,PM_p4 )
/** \brief pm_assert5

*/
#define pm_assert5( fmt,PM_p1,PM_p2,PM_p3,PM_p4,PM_p5 ) pm_log_wrap_fmt( pm_log_level_assert,__FILE__,__LINE__,fmt,PM_p1,PM_p2,PM_p3,PM_p4,PM_p5 )

/** \brief pm_debug0

*/
#define pm_debug0( fmt ) pm_log_wrap( pm_log_level_debug,__FILE__,__LINE__,fmt )
/** \brief pm_debug1

*/
#define pm_debug1( fmt,PM_p1 ) pm_log_wrap_fmt( pm_log_level_debug,__FILE__,__LINE__,fmt,PM_p1 )
/** \brief pm_debug2

*/
#define pm_debug2( fmt,PM_p1,PM_p2 ) pm_log_wrap_fmt( pm_log_level_debug,__FILE__,__LINE__,fmt,PM_p1,PM_p2 )
/** \brief pm_debug3

*/
#define pm_debug3( fmt,PM_p1,PM_p2,PM_p3 ) pm_log_wrap_fmt( pm_log_level_debug,__FILE__,__LINE__,fmt,PM_p1,PM_p2,PM_p3 )
/** \brief pm_debug4

*/
#define pm_debug4( fmt,PM_p1,PM_p2,PM_p3,PM_p4 ) pm_log_wrap_fmt( pm_log_level_debug,__FILE__,__LINE__,fmt,PM_p1,PM_p2,PM_p3,PM_p4 )
/** \brief pm_debug5

*/
#define pm_debug5( fmt,PM_p1,PM_p2,PM_p3,PM_p4,PM_p5 ) pm_log_wrap_fmt( pm_log_level_debug,__FILE__,__LINE__,fmt,PM_p1,PM_p2,PM_p3,PM_p4,PM_p5 )



#define pm_trace0( fmt ) pm_log_wrap( pm_log_level_trace,__FILE__,__LINE__,fmt )
#define pm_trace1( fmt,PM_p1 ) pm_log_wrap_fmt( pm_log_level_trace,__FILE__,__LINE__,fmt,PM_p1 )
#define pm_trace2( fmt,PM_p1,PM_p2 ) pm_log_wrap_fmt( pm_log_level_trace,__FILE__,__LINE__,fmt,PM_p1,PM_p2 )
#define pm_trace3( fmt,PM_p1,PM_p2,PM_p3 ) pm_log_wrap_fmt( pm_log_level_trace,__FILE__,__LINE__,fmt,PM_p1,PM_p2,PM_p3 )
#define pm_trace4( fmt,PM_p1,PM_p2,PM_p3,PM_p4 ) pm_log_wrap_fmt( pm_log_level_trace,__FILE__,__LINE__,fmt,PM_p1,PM_p2,PM_p3,PM_p4 )
#define pm_trace5( fmt,PM_p1,PM_p2,PM_p3,PM_p4,PM_p5 ) pm_log_wrap_fmt( pm_log_level_trace,__FILE__,__LINE__,fmt,PM_p1,PM_p2,PM_p3,PM_p4,PM_p5 )

const char* pm_get_source_name( const char* pszsrc );
PM_INT pm_log_open_ex( PM_LPCSTR pszcLogname, PM_BOOL bDisableDaily, PM_BOOL bRecordSource );
PM_INT pm_log_get_level();

int pm_log_context_enable( void(* free_callback)(void*) );
int pm_log_set_context( void* pcontext );
void* pm_log_get_context();

typedef PM_HANDLE (*PM_LOG_PF_OPEN) ( time_t tnow );
typedef void( *PM_LOG_PF_WRITE)( PM_HANDLE fp, ByteStr* bs );
typedef void(* PM_LOG_PF_CLOSE) ( PM_HANDLE fp );
typedef void(* PM_LOG_PF_FORMAT) ( time_t tnow, char* prefix, int level);

void pm_log_set_io_method( PM_LOG_PF_OPEN o, PM_LOG_PF_WRITE w, PM_LOG_PF_CLOSE c, PM_LOG_PF_FORMAT f );


extern void pm_log_close_file( PM_HANDLE fp );
extern PM_HANDLE pm_log_open_file( time_t tnow );
extern void pm_log_format( time_t tnow, char *buf, int level, const char *fmt, int with_crln);
extern void pm_log_output_to_string( ByteStr* bs, time_t tnow, int level, const char *place, char *buf, va_list args, int with_crln );
extern void pm_log_write_file( PM_HANDLE fp, ByteStr* bs, int level );
extern void pm_log_output( PM_HANDLE fp, time_t tnow, int level, const char *place, char *buf, va_list args, int with_crln );
//extern void pm_log_output_new( time_t tnow, int level, const char *place, char *buf, va_list args, int with_crln , char *outbuf);

#ifdef __cplusplus
}
#endif

#endif
