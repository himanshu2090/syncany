#ifndef __PM_PROCESS_H__
#define __PM_PROCESS_H__

#ifdef __cplusplus
extern "C"
{
#endif

PM_HANDLE	pm_create_lock();
void		pm_destroy_lock(PM_HANDLE);

int			pm_lock( PM_HANDLE );
int			pm_unlock( PM_HANDLE );


#ifdef __cplusplus
}
#endif


#endif




