#ifndef __PM_PROCESS_CPP_H__
#define __PM_PROCESS_CPP_H__

/**	
 *@file PmProcess.h 
 *@brief 线程，锁
*/

/**	\brief	Redefinition of PM_UINT as (*PM_CALL_BACK_FUNCTION)(PM_LPVOID)
*/
typedef PM_UINT (*PM_CALL_BACK_FUNCTION)(PM_LPVOID);
/**	\brief	Redefinition of enum as tagPM_THREAD_RELEASE_METHOD
*/
typedef enum tagPM_THREAD_RELEASE_METHOD
{
	PM_TRM_NORMAL,
	PM_TRM_WAIT,
	PM_TRM_TERMINATE,
}PM_THREAD_RELEASE_METHOD;
/**	\brief PmBeginThread
*/
PmThreadPtr PmBeginThread( PM_CALL_BACK_FUNCTION,PM_LPVOID para = PM_NULL,PM_BOOL bSuspend = PM_FALSE, PM_THREAD_RELEASE_METHOD method = PM_TRM_NORMAL, int nWaitSeconds = 5 );

/** \brief 封装了CriticalSection的类
* \remarks 使用时，最好用宏KVLOCK和KVUNLOCK来进行加锁解锁操作
*/
#define PMLOCK(l) try{l->lock()
/**	\brief	The PMUNLOCK function
	
*/
#define PMUNLOCK(l) }catch(PmException ex){l->unlock();throw ex;} catch(...){ l->unlock(); throw; } l->unlock()

/**	\brief	The PmAutoLock class

*/
class PmAutoLock
{
public:
	PmAutoLock(PmSynchronizerPtr& ptrSynchronizer):
		m_ptrSynchronizer(ptrSynchronizer)
	{
		ptrSynchronizer->wait();
	}
	~PmAutoLock(){
		m_ptrSynchronizer->signal();
	}
private:
	PmSynchronizerPtr &m_ptrSynchronizer;
};

/**	\brief	The PM_AUTO_LOCK function
*/
#define PM_AUTO_LOCK( __lock__ ) PmAutoLock __autolock__##__lock__( __lock__ )

/**	\brief	Redefinition of enum as tag_PM_SYNCHRONIZER_TYPE
*/
typedef enum tag_PM_SYNCHRONIZER_TYPE
{
	PM_SYN_MUTEX,
	PM_SYN_SEMAPHORE
}PM_SYNCHRONIZER_TYPE;

/**	\brief	The PmGetSynchronizer function

	\param	type	a parameter of type PM_SYNCHRONIZER_TYPE

	\return	PmSynchronizerPtr

	
*/
PmSynchronizerPtr	PmGetSynchronizer(PM_SYNCHRONIZER_TYPE type = PM_SYN_MUTEX);
/**	\brief	The PmGetSemaphore function

	\param	nMaxCount	a parameter of type PM_INT

	\return	PmSynchronizerPtr

	
*/
PmSynchronizerPtr	PmGetSemaphore(PM_INT nMaxCount);
/**	\brief PmGetCondition
*/
PmConditionPtr		PmGetCondition(PM_VOID);
/**	\brief	The PmGetProcessLock function

	\param	pszLockName	a parameter of type const PM_CHAR*

	\return	PmSynchronizerPtr

	
*/
PmSynchronizerPtr	PmGetProcessLock( const PM_CHAR* pszLockName );
#ifdef WIN32

void PmDumpSetPath( const char* pszPath );
void PmDumpEnableDebug( PM_BOOL bEnable );
void PmDumpEnable( PM_BOOL bMessageBoxWhenCrash );

#endif

PM_HANDLE PmCreateEvent();

#define PM_EVENT_NOT_HAPPENED	0
#define PM_EVENT_HAPPENED		1
#define PM_EVENT_ERROR			2
/*	\return int 
	返回0, 表示事件还未发生
	返回1, 表示事件已经发生
	返回-1,表示错误发生
	调用后立即返回
*/

int PmCheckEvent( PM_HANDLE );
int PmSetEvent( PM_HANDLE );
int PmCloseEvent( PM_HANDLE );

PmProcessPtr pm_spawn( const char* pszFile, char*const  argv[] );

PmProcessExPtr pm_spawn_ex( const char* args[], ByteStr* bsIn = PM_NULL, PM_BOOL bOneOutput = PM_FALSE );

#endif //__PM_PROCESS_CPP_H__
