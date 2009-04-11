#ifndef __PMKERNEL_PRIVATE__H__
#define __PMKERNEL_PRIVATE__H__

#include "apr/apr_hash.h"
#ifdef WIN32
#include <windows.h>
#else
#include <errno.h>
#endif

#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>

#ifdef WIN32
#include <Windows.h>
#include <Tlhelp32.h>
#include <Psapi.h>
#include <shlwapi.h>
#include <ShellAPI.h>
#include <winsock2.h>
#pragma comment(lib,"shlwapi.lib")
#pragma comment(lib,"ws2_32.lib")
#else
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <utime.h>
#include <fcntl.h>
#include <dirent.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/sysinfo.h>
#endif

#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>

#include "pcre/pcre.h"

#include "pmkernel.h"
#include "pm_bytestr_def.h"
#include "pm_bytestr_charset.h"
#include "pm_format.h"

#include "apr/apr_pools.h"
#include "apr/apr_thread_proc.h"
#include "apr/apr_shm.h"
#ifdef WIN32
    #pragma comment(lib,"libapr-1.lib")
#else
    #include <sys/wait.h>
    #include <libgen.h>
#endif

#endif
