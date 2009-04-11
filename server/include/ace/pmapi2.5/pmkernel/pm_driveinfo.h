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
	
PM_INT PmGetMemorySize();//获取系统物理内存大小，单位为M
PM_UINT PmGetMemoryUsage();//获取系统内存使用的百分比
PM_INT PmGetDriveAvailSize(char* drive);//获取drive路径所在硬盘的总空间大小
int PmGetDriveUsage(char* drive);//获取drive路径所在硬盘的空间使用百分比
PM_UINT PmGetLocalCPUCount(void);//获取系统中CPU个数
PM_UINT PmGetCPUFrequency(void);//获取CPU的主频,这个函数需要改进，需要好好考虑多个CPU的情况
PM_INT PmGetCPULoad(void);//获取当前系统CPU资源的使用百分比

#ifdef __cplusplus
}
#endif
#endif

