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
///PM_TIME PMAPI��ʱ��ṹ��������tm�ṹ
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

///PM��ʱ�����ͣ�������time_t����PM_TIME_T��64λ�ģ���time_t��32λ��
typedef PM_INT64 PM_TIME_T;	 

/**
 *\brief	pm_time��ȡ��ǰʱ��,�ɲο�time����
 *\return	��ǰʱ�䣬����Ϊ��λ����1970-1-1 0:0:0��ʼ��ʱ�������ŵ�����	
 */
PM_TIME_T pm_time();


/**	
 *\brief	pm_localtime ��ʱ��ֵת���ɵ���ʱ��
 *\param[in]	pmTimeSrc	PM_TIME_T���͵�ʱ��ֵ
 *\param[out]	pmTimeResult ת����ķ���ֵ
 *\return	ת���ɹ������ط���ֵ��ʧ�ܷ���PM_FALSE
 */
PM_BOOL pm_localtime( PM_TIME_T pmTimeSrc,  PM_TIME* pmTimeResult);

/**	
 *\brief	pm_mktime ��PM_TIME���͵�ʱ��ֵת���ɳ����͵�ʱ��ֵ
 *\param[in]	pmTimeSrc	PM_TIME���͵�ʱ��ֵ
 *\param[out]	pmTimeResult ת����ķ���ֵ
 *\return	ת���ɹ������ط���ֵ��ʧ�ܷ���PM_FALSE
 */
PM_BOOL pm_mktime( const PM_TIME* pmTimeSrc, PM_TIME_T* pmTimeResult );

/**	
 *\brief	pm_difftime ��ȡ����ʱ���
 *\param[in]	time1 PM_TIME_T���͵�ʱ��ֵ
 *\param[in]	time0 PM_TIME_T���͵�ʱ��ֵ
 *\return	����time1��time0��ʱ���
 */
PM_DOUBLE pm_difftime(PM_TIME_T time1, PM_TIME_T time0);

/**	
 *\brief	pm_strftime ��ʽ�����һ��ʱ�䴮
 *\param[out]	pszDest �����ʱ���ַ���
 *\param[in]	uiMaxSize	pszDest�������Ĵ�С
 *\param[in]	lpszcFormat	ʱ���ַ����ĸ�ʽ
 *\param[in]	timeptr	PM_TIME*���͵�ʱ��ֵ
 *\return	���ɹ����򷵻ظ�ʽ���������ַ����ȣ���������������pszDest����ʱ���ַ�����
 *��uiMaxSizeС�ڸ�ʽ���������ĳ���,����ֵ��0��pszDest�����ݲ�ȷ����
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

