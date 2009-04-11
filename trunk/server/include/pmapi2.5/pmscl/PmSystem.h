#if !defined(__PmSystem__h__2006915)
#define __PmSystem__h__2006915

/**	\brief	The PmSystem class

*/
//class PmSystem
//{
//public:
//	static PmPrintStream err;
//	static PmPrintStream out;
////	static PmInputStream in;
//};

struct PmPerformanceContext
{
	const char* pszInfo;
	const char* pszFileName;
	int nLine;
};

class PmPerformanceTicker : public PmPerformanceContext
{//性能分析计数器
public:
	PmPerformanceTicker( const char* _pszInfo, const char* _pszFileName, int _nLine);
	virtual ~PmPerformanceTicker();
	void clear();
	void tickBegin();
	void tickEnd();
	void report();
	PM_INT64 getTickCount();
private:
	PM_INT64 m_liTimestamp;
	PM_INT64 m_iTick;
};


class PmPerformanceAutoTicker
{
public:
	PmPerformanceAutoTicker(PmPerformanceTicker& ticker);
	void clear();
	~PmPerformanceAutoTicker();
private:
	PmPerformanceTicker* m_pTicker;
};
struct PmPerformanceReportorData;
class PmPerformanceReportor : PmPerformanceContext
{
public:
	PmPerformanceReportor( const char* _pszInfo, const char* _pszFileName, int _nLine);
	~PmPerformanceReportor();
	void report();
	void add( PmPerformanceTicker* pTicker );
	PM_DWORD getId();
private:
	PmPerformanceReportorData *m_pData;
};
struct PmPerformanceReportorInfo
{
	PM_DWORD dwId;
	PmPerformanceReportor* pReportor;
};

PM_BOOL PmpaHasReportorRegistered();
PmPerformanceReportorInfo* PmpaGetLastReportor();



#endif
