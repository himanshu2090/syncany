#ifndef __PM_TIME_H__
#define __PM_TIME_H__ 
/**	
 *@file PmTime.h
 *@brief PmTime所在文件
*/

/**	\brief	The PmTime class

*/

class PmTime
{

public:

// Constructors
	static PmTime getCurrentTime();


	PmTime();
	PmTime(PM_TIME_T time);
	PmTime(PM_INT nYear, PM_INT nMonth, PM_INT nDay, PM_INT nHour, PM_INT nMin, PM_INT nSec);
	PmTime(const PmTime& timeSrc);

	/**	\brief	load 从时间戳加载时间
		\param	pmstrTime	时间串，格式为"Fri, 16 Sep 2006 hh:mm:ss +800"
		\return	失败返回PM_FALSE,成功返回PM_TRUE		
	*/
	PM_BOOL load(PmString pmstrTime);


	/**	\brief	loadfromTimeStamp 从时间戳加载时间
		\param	pmstrTimeStamp	时间戳，格式为YYYYMMDDhhmmss
		\return	失败返回PM_FALSE,成功返回PM_TRUE
	*/
	PM_BOOL loadfromTimeStamp(PmString pmstrTimeStamp);
	PM_BOOL loadfromUnixTimeStamp(PmString pmstrUnixTimeStamp);


	const PmTime& operator=(const PmTime& timeSrc);
	const PmTime& operator=(PM_TIME_T t);



// Attributes
	PM_TIME getGmtTm() const;
	PM_TIME getLocalTm() const;


	PM_TIME_T getTime() const;
	PM_INT getYear() const;
	PM_INT getMonth() const;       // month of year (1 = Jan)
	PM_INT getDay() const;         // day of month
	PM_INT getHour() const;
	PM_INT getMinute() const;
	PM_INT getSecond() const;
	PM_INT getDayOfWeek() const;   // 1=Sun, 2=Mon, ..., 7=Sat


	// formatting using "C" strftime
//	%a 
//	Abbreviated weekday name 
//	%A 
//	Full weekday name 
//	%b 
//	Abbreviated month name 
//	%B 
//	Full month name 
//	%c 
//	Date and time representation appropriate for locale 
//	%d 
//	Day of month as decimal number (01 – 31) 
//	%H 
//	Hour in 24-hour format (00 – 23) 
//	%I 
//	Hour in 12-hour format (01 – 12) 
//	%j 
//	Day of year as decimal number (001 – 366) 
//	%m 
//	Month as decimal number (01 – 12) 
//	%M 
//	Minute as decimal number (00 – 59) 
//	%p 
//	Current locale's A.M./P.M. indicator for 12-hour clock 
//	%S 
//	Second as decimal number (00 – 59) 
//	%U 
//	Week of year as decimal number, with Sunday as first day of week (00 – 53) 
//	%w 
//	Weekday as decimal number (0 – 6; Sunday is 0) 
//	%W 
//	Week of year as decimal number, with Monday as first day of week (00 – 53) 
//	%x 
//	Date representation for current locale 
//	%X 
//	Time representation for current locale 
//	%y 
//	Year without century, as decimal number (00 – 99) 
//	%Y 
//	Year with century, as decimal number 
//	%z, %Z 
//	Time-zone name or abbreviation; no characters if time zone is unknown 

	PmString format(PM_LPCSTR pFormat) const;
	PmString formatGmt(PM_LPCSTR pFormat) const;
	PM_VOID dump_time();
	PmString getTimeStamp();
	
	///Date:Fri, 31 Jun 2005 23:59:59 +0800
	PM_VOID unload(PmString & pmstrTime);

	//YYYY-MM-DD hh:mm:ss
	PmString getDataBaseFormat();

	friend PmTime operator-(const PmTime& tm1, const PM_INT iSecond);
	friend PmTime operator+(const PmTime& tm1, const PM_INT iSecond);
	friend PM_BOOL operator == (const PmTime& tm1, const PmTime& tm2);


//	PmString getRfcDate();//


private:
	PM_TIME_T m_time;	 
};

/**	\brief	PmTime operator-

*/
inline PmTime operator-(const PmTime& tm1, const PM_INT iSecond)
{
	if(tm1.m_time-iSecond<0)
	{
		PmTime temp(0);
		return 	temp;
	}
	PmTime temp(tm1.m_time-iSecond);
	return temp; 
}
/**	\brief	PmTime operator+

*/
inline PmTime operator+(const PmTime& tm1, const PM_INT iSecond)
{
	PmTime temp(tm1.m_time+iSecond);
	return temp; 
}
/**	\brief	PmTime operator==

*/
inline PM_BOOL operator == (const PmTime& tm1, const PmTime& tm2)
{
	if(tm1.m_time == tm2.m_time) 
		return PM_TRUE;
	else
		return PM_FALSE;
}


#endif

