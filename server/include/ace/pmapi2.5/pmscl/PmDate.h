#if !defined(__PmDate__h__2006829)
#define __PmDate__h__2006829

/**	\brief	The PM_DATE struct

*/
struct PM_DATE {
	PM_INT year;
	PM_INT month;
	PM_INT day;

	PM_INT hour;
	PM_INT minute;
	PM_INT second;

	PM_INT timezone;
	PM_INT week;

	void dump_date(PM_DATE &date);
	void init_date(PM_DATE &date);
};

/**	\brief	The PmDateParser class

*/
class PmDateParser {
protected:
	/*!
		\brief 如果是星期就返回星期所对应的数字，失败返回-1, 星期天是0
	*/
	int is_week(PM_LPCSTR str);

	/*!
	    \brief 判断是否为日
	*/
	int is_day(PM_LPCSTR str);

	/*!
	    \brief 判断是否为字母月份
	*/
	int is_month(PM_LPCSTR str);

	/*!
	    \brief 判断是否为数字月份
	*/
	int is_digit_month(PM_LPCSTR str);

	/*!
	    \brief 判断是否为年
	*/
	int is_year(PM_LPCSTR str);

	/*!
	    \brief 判断是否为时区
	*/
	int is_timezone(PM_LPCSTR str);

	/*!
	    \brief 判断是否为时间
	*/
	int is_time(PM_LPCSTR str, int &refHour, int &refMinute, int &refSecond); 
public:
	PM_DATE parse(PmString strDate);
};

inline int PmDateParser::is_week(PM_LPCSTR str)
{
	/* Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday */
	PM_LPCSTR cp;
	int len = pm_strlen(str);
	if (len < 3) {
		return -1;
	}

	cp = str;
	switch(*cp) {
	case 'S':
		if (pm_strncmp("Sunday", str, pm_strlen(str)) == 0) {
			return 0;
		}
		else if (pm_strncmp("Saturday", str, pm_strlen(str)) == 0) {
			return 6;
		}
		break;
	case 'M':
		if (pm_strncmp("Monday", str, pm_strlen(str)) == 0) {
			return 1;
		}
		break;
	case 'T':
		if (pm_strncmp("Tuesday", str, pm_strlen(str)) == 0) {
			return 2;
		}
		else if (pm_strncmp("Thursday", str, pm_strlen(str)) == 0) {
			return 4;
		}
		break;
	case 'W':
		if (pm_strncmp("Wednesday", str, pm_strlen(str)) == 0) {
			return 3;
		}
		break;
	case 'F':
		if (pm_strncmp("Friday", str, pm_strlen(str)) == 0) {
			return 5;
		}
		break;
	}

	return -1;
}
inline int PmDateParser::is_year(PM_LPCSTR str) 
{
	int len = pm_strlen(str);
	if (len != 4 ) {
		return -1;
	}

	for(int i=0; i<4; i++) {
		if (str[i]<'0' || str[i]>'9') {
			return -1;
		}
	}
	
	int d = pm_atoi(str);

	return d;
}

inline int PmDateParser::is_month(PM_LPCSTR str)
{
	int len = pm_strlen(str);
	if (len < 3) {
		return -1;
	}

	PM_LPCSTR months[] = {
		"January", "February", "March", "April", "May", "June", "July", 
		"August", "September", "October", "November", "December"};

	PM_LPCSTR *sp = months;
	for(int i=1;i<13;i++) {
		if (pm_strncmp(*sp++, str, len) == 0) {
			return i;
		}
	}

	return -1;
}

inline int PmDateParser::is_day(PM_LPCSTR str) 
{
	int len = pm_strlen(str);
	if (len > 2 || len <1) {
		return -1;
	}
	
	int d = pm_atoi(str);
	if (d>31 || d<1) {
		return -1;
	}

	return d;
}

inline int PmDateParser::is_time(PM_LPCSTR str, int &refHour, int &refMinute, int &refSecond)
{
	char* cp = (char*)str;
	int h, m, s;

	if (pm_strlen(str) != 8) {
		return -1;
	}

	if (cp[0] <'0' || cp[0]>'9') {
		return -1;
	}
	if (cp[1] <'0' || cp[1]>'9') {
		return -1;
	}
	if (cp[2] != ':' ) {
		return -1;
	}
	if (cp[3] <'0' || cp[3]>'9') {
		return -1;
	}
	if (cp[4] <'0' || cp[4]>'9') {
		return -1;
	}
	if (cp[5] != ':') {
		return -1;
	}
	if (cp[6] <'0' || cp[6]>'9') {
		return -1;
	}
	if (cp[7] <'0' || cp[7]>'9') {
		return -1;
	}

	h = pm_atoi(&cp[0]);
	m = pm_atoi(&cp[3]);
	s = pm_atoi(&cp[6]);
	
	if (h>24 || h<0) {
		return -1;
	}

	if (m>59 || m<0) {
		return -1;
	}

	if (s>59 || s<0) {
		return -1;
	}

	refHour = h;
	refMinute = m;
	refSecond = s;

	return 0;
}

inline int PmDateParser::is_timezone(PM_LPCSTR str) 
{
	PM_LPCSTR cp = str;
	if (*cp != '+' || *cp != '-') {
		return -1;
	}

	int t = pm_atoi(cp+1);
	if (*cp == '-') {
		return 0 - t;
	}
	return t;
}

inline int PmDateParser::is_digit_month(PM_LPCSTR str) {
	int len = pm_strlen(str);
	if (len > 2 || len <1) {
		return -1;
	}
	
	int m = pm_atoi(str);
	if (m>12 || m<1) {
		return -1;
	}
	
	return m;
}

void inline PM_DATE::init_date(PM_DATE& date)
{
	date.day = -1;
	date.hour = -1;
	date.year = -1;
	date.month = -1;
	date.second = -1;
	date.minute = -1;
	date.timezone = -256;
	date.week = -1;
}

void inline PM_DATE::dump_date(PM_DATE &date)
{
	pm_printf("%04d-%02d-%02d %02d:%02d:%02d %d %d", 
		date.year, date.month, date.day,
		date.hour, date.minute, date.second, 
		date.week, date.timezone);
}

inline PM_DATE PmDateParser::parse(PmString strDate)
{
	PM_DATE date;

	//加入日期 格式如下：Date:Fri,31 Jun 2005 23:59:59 +0800\r\n
	PM_LPCSTR szDate = strDate.c_str();

	/* 字母只在两种情况下存在，一个是月份，一个是星期 */
	PM_CHAR* buf = (PM_CHAR*)pm_malloc(strDate.length());
	PM_LPCSTR cp;
	PM_CHAR* pBufBegin;
	PM_CHAR* pBuf;

	date.init_date(date);

	cp = szDate;
	pBuf = pBufBegin = buf;
	int n;
	for(;*cp;) {
		*pBuf++ = *cp++;
		if (*cp == ',' || *cp == ' ') {
			*pBuf = '\0';
			if ((n=is_week(pBufBegin)) > -1) {
				date.week = n;
			}
			else if ((n=is_month(pBufBegin))>0) {
				date.month = n;
			}
			else if ((n=is_day(pBufBegin))>0) {
				date.day = n;
			}
			else if ((n=is_year(pBufBegin))>0) {
				date.year = n;
			}
			else if ((n=is_timezone(pBufBegin))>0) {
				date.timezone = n;
			}
			else if ((n=is_time(pBufBegin, date.hour, date.minute, date.second))==0) {
//				m_strTime = pBegin;
			}
			else {
				PmThrowException(PM_HERE, PM_ERROR_UNKNOWN, "date formate can't be recognized");
			}
			pBuf++;
			pBufBegin = pBuf;
			cp++;
		}
	}

	pm_free(buf);
	
	date.dump_date(date);
	return date;
}

#endif
