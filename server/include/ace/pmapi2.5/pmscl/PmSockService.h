#ifndef __PM_SOCK_SERVICE_H__
#define __PM_SOCK_SERVICE_H__
//
typedef int(*PM_ON_CONNECT_FUNCTION)( PmTcpSocket*, PM_HANDLE hStopEvent, PM_HANDLE hData );
////if windows, call pm_sock_init first

PmServicePtr PmCreateSocketService( PM_UINT uiPort, PM_UINT uiTimeOut, PM_UINT uiCount,PM_ON_CONNECT_FUNCTION pf );

#endif
