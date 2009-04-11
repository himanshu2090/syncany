#ifndef __PM__TIME__
#define __PM__TIME__

/**	
 *@file pm_time.h 
 *@brief 
*/


#ifdef __cplusplus
extern "C"
{
#endif
///PM_TIME PMAPI的时间结构，类似与tm结构
typedef struct pm_time {
        PM_INT tm_sec;     ///< seconds after the minute - [0,59] 
        PM_INT tm_min;     ///< minutes after the hour - [0,59] 
        PM_INT tm_hour;    ///< hours since midnight - [0,23] 
        PM_INT tm_mday;    ///< day of the month - [1,31] 
        PM_INT tm_mon;     ///< months since January - [0,11] 
        PM_INT tm_year;    ///< years since 1900 
        PM_INT tm_wday;    ///< days since Sunday - [0,6] 
        PM_INT tm_yday;    ///< days since January 1 - [0,365] 
        PM_INT tm_isdst;   ///< daylight savings time flag 
 }PM_TIME;

///PM的时间类型，类似与time_t，但PM_TIME_T是64位的，而time_t是32位的
typedef PM_INT64 PM_TIME_T;	 

/**
 *\brief	pm_time获取当前时间,可参考time函数
 *\return	当前时间，以秒为单位，从1970-1-1 0:0:0开始计时，所流逝的秒数	
 */
PM_TIME_T pm_time();


/**	
 *\brief	pm_localtime 把时间值转换成当地时间
 *\param[in]	pmTimeSrc	PM_TIME_T类型的时间值
 *\param[out]	pmTimeResult 转换后的返回值
 *\return	转换成功，返回非零值，失败返回PM_FALSE
 */
PM_BOOL pm_localtime( PM_TIME_T pmTimeSrc,  PM_TIME* pmTimeResult);

/**	
 *\brief	pm_mktime 把PM_TIME类型的时间值转换成长整型的时间值
 *\param[in]	pmTimeSrc	PM_TIME类型的时间值
 *\param[out]	pmTimeResult 转换后的返回值
 *\return	转换成功，返回非零值，失败返回PM_FALSE
 */
PM_BOOL pm_mktime( const PM_TIME* pmTimeSrc, PM_TIME_T* pmTimeResult );

/**	
 *\brief	pm_difftime 求取两个时间差
 *\param[in]	time1 PM_TIME_T类型的时间值
 *\param[in]	time0 PM_TIME_T类型的时间值
 *\return	返回time1和time0的时间差
 */
PM_DOUBLE pm_difftime(PM_TIME_T time1, PM_TIME_T time0);

/**	
 *\brief	pm_strftime 格式化输出一个时间串
 *\param[out]	pszDest 输出的时间字符串
 *\param[in]	uiMaxSize	pszDest缓冲区的大小
 *\param[in]	lpszcFormat	时间字符串的格式
 *\param[in]	timeptr	PM_TIME*类型的时间值
 *\return	若成功，则返回格式化输出后的字符长度，不包括结束符，pszDest内是时间字符串；
 *若uiMaxSize小于格式化输出所需的长度,返回值是0，pszDest的内容不确定。
 */
PM_UINT pm_strftime( PM_LPSTR pszDest, PM_UINT uiMaxSize, PM_LPCSTR lpszcFormat,	const PM_TIME *timeptr);


/**	\brief	The pm_gmtime function

	\param	pmTimeSrc	a parameter of type const PM_TIME_T
	\param	pmStructTime	a parameter of type PM_TIME*

	\return	PM_VOID

	
*/
PM_VOID pm_gmtime(const PM_TIME_T pmTimeSrc ,PM_TIME* pmStructTime);



#ifdef __cplusplus
}
#endif


#endif

