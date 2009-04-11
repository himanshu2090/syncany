#ifndef __PM_SCL_H__
#define __PM_SCL_H__

/**	
 *@file pmscl.h 
 *@brief pmscl层的各个.h文件
*/

//#include "pmInterface.h"
#pragma warning(disable:4251)

/** \brief ADD_EXT_CLASS
*/
#ifdef ADDDLL_EXPORTS
	#define ADD_EXT_CLASS __declspec(dllexport)
#else
	#define ADD_EXT_CLASS __declspec(dllimport)
#endif  //ADD_EXT_CLASS


#include "PmObject.h"
#include "PmPrintable.h"

#include "PmError.h"
#include "PmUri.h"
#include "PmInterface.h"
#include "PmComSmartPtr.h"
#include "PmSimpleSmartPtr.h"
#include "PmSmartPtr.h"
#include "PmInterfaceDef.h"
#include "PmByteStream.h"
#include "PmPath.h"
#include "PmFile.h"
#include "PmSock.h"
#include "PmStream.h"
#include "pmdb.h"
#include "PmPublic.h" 
#include "PmMessageQueue.h"
#include "PmVariantPtr.h"
#include "PmDbColumn.h"
#include "PmDbQRecord.h"
#include "PmProcess.h"
#include "PmSockService.h"
#include "PmSock.h"
#include "PmTime.h"
#include "PmDbQueue.h"
#include "PmArgs.h"
#include "PmDir.h"
#include "PmModule.h"
#include "PmSystem.h"
#include "PmMQ.h"
#include "PmPool.h"
#include "PmArgParser.h"
#include "PmMemCache.h"
//#include "mig_log.h"
//#include "mig_occ.h"

//#define PM_CLIENT_SOCKET "PmSock"
//#define PM_CLIENT_SSLSOCKET "SSLPmSock"

#ifdef WIN32
	#ifndef PMSCL_EXPORTS
		#ifdef _DEBUG
			#pragma comment(lib,"pmsclD.lib")
		#else
			#pragma comment(lib,"pmscl.lib")
		#endif
	#endif
#endif

#endif

