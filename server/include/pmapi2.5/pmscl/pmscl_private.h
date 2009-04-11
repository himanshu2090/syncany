#ifndef __PM_SCL_PRIVATE__H___
#define __PM_SCL_PRIVATE__H___


#include "../pmkernel/pmkernel.h"

#include "pmscl.h"

#include "PmApi.h"

#include <map>
#include <string>
#include <vector>
#include <list>
#include <set>
using namespace std;

#ifdef WIN32
    #include <windows.h>
#else
    #include <dlfcn.h>
    #include <pthread.h>
#endif

#include "apr/apr_general.h"
#include "apr/apr_pools.h"
#include "apr/apr_errno.h"
#include "apr/apr_dso.h"
#include "apr/apr_strings.h"
#include "apr/apr_file_info.h"
#include "apr/apr_date.h"
#include "apr/apr_file_io.h"
#include "apr/apr_file_info.h"
#include "apr/apr_errno.h"
#include "apr/apr_general.h"
#include "apr/apr_lib.h"
#include <stdlib.h>
#include "apr/apr.h"
#if APR_HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef USE_APR
#pragma comment(lib,"libapr-1.lib")
#pragma comment(lib,"libaprutil-1.lib")
#pragma comment(lib,"libapriconv-1.lib")
#endif
#include "PmApr.h"


#ifdef WIN32
#include <process.h>
#include <windows.h>

#define PMPROC_CLOSE_HANDLE( _h_ ) do{ if( _h_ ) CloseHandle( _h_ ) ;}while(0)

#else
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define MAX_PATH  1024
#endif
#include "PmStreamDef.h"
#include "PmArgParser.h"

#include "../pmkernel/pm_bytestr_charset.h"
#include "libmemcached/memcached.h"

#ifdef WIN32
    #include "winsock2.h"
#else
    #include <sys/select.h>
    #include <sys/time.h>
    #include <errno.h>
    #include <sys/types.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #include <unistd.h>
    #include <poll.h>
#endif

#include "../pmkernel/pmkernel.h"
#include "pmscl.h"
#include "PmSock.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>

#pragma comment(lib,"ssleay32.lib")
#pragma comment(lib,"libeay32.lib")


#endif

