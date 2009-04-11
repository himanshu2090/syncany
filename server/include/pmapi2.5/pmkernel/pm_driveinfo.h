#ifndef __PM__DRIVERINFO__
#define __PM__DRIVERINFO__

/**	
 *@file pm_driveinfo.h
 *@brief 
*/


#ifdef __cplusplus
extern "C"
{
#endif
	
PM_INT PmGetMemorySize();//��ȡϵͳ�����ڴ��С����λΪM
PM_UINT PmGetMemoryUsage();//��ȡϵͳ�ڴ�ʹ�õİٷֱ�
PM_INT PmGetDriveAvailSize(char* drive);//��ȡdrive·������Ӳ�̵��ܿռ��С
int PmGetDriveUsage(char* drive);//��ȡdrive·������Ӳ�̵Ŀռ�ʹ�ðٷֱ�
PM_UINT PmGetLocalCPUCount(void);//��ȡϵͳ��CPU����
PM_UINT PmGetCPUFrequency(void);//��ȡCPU����Ƶ,���������Ҫ�Ľ�����Ҫ�úÿ��Ƕ��CPU�����
PM_INT PmGetCPULoad(void);//��ȡ��ǰϵͳCPU��Դ��ʹ�ðٷֱ�

#ifdef __cplusplus
}
#endif
#endif

