#ifndef __PM_FORMAT_H__
#define __PM_FORMAT_H__
//本文件不对外公开
#ifdef __cplusplus
extern "C"
{
#endif

/**	\brief	The format struct

*/
typedef struct pm_format
{
	PM_INT minus;
	PM_INT zero;

	PM_ULONG min_width;

	PM_INT has_prec;
	PM_LONG prec;

	PM_LONG type;
}PM_FORMAT;
PM_VOID pm_format_flags( PM_FORMAT* fmt_data, PM_LPCSTR * fmt);
PM_VOID pm_format_width( PM_FORMAT* fmt_data, PM_LPCSTR * fmt, va_list* args);
PM_VOID pm_format_prec( PM_FORMAT* fmt_data, PM_LPCSTR * fmt, va_list* args);
PM_VOID pm_format_type( PM_FORMAT* fmt_data, PM_LPCSTR * fmt);

#ifdef __cplusplus
}
#endif

#endif

