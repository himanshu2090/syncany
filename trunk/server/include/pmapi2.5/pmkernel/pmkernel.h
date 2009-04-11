#ifndef __PMKERNEL_H__
#define __PMKERNEL_H__

/**	
 *@file pmkernel.h 
 *@brief pmkernel层的所有头文件
*/

#include "pm_types.h"
#include "pm_bytestr_def.h"
#include "pm_platform.h"
#include "pm_error.h"
#include "pm_assert.h"
#include "pm_debug.h"

#include "pm_time.h"
#include "pm_sdk.h"
#include "pm_sock.h"
#include "pm_encoding.h"
#include "pm_bytestr.h"
#include "pm_fileio.h"
#include "pm_log.h"
#include "pm_pubfun.h"
#include "pm_list.h"
#include "pm_process.h"

#include "pm_driveinfo.h"

#include "pm_performance.h"

#ifdef WIN32
	#ifndef PMKERNEL_EXPORTS
		#ifdef _DEBUG
			#pragma comment(lib,"pmkernelD.lib")
		#else
			#pragma comment(lib,"pmkernel.lib")
		#endif
	#endif
#endif

#endif

