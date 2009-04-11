#ifndef __PM_DB_DEF_H__
#define __PM_DB_DEF_H__

#include "../pmkernel/pmkernel.h"
#include "../pmscl/pmscl.h"

#ifdef WIN32
	#include <windows.h>
#else
	#include <unistd.h>
#endif
#include <mysql/mysql.h>
#include <stdio.h>
#include <string>
using namespace std;

#include "PmMySQLResultSet.h"
#include "PmMySQLDBConn.h"

#endif
