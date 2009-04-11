#ifndef __PM_PERFORMANCE_H__
#define __PM_PERFORMANCE_H__

//#define PM_PERFORMANCE_ANA

#ifdef __cplusplus
extern "C"
{
#endif


int pm_perm_init();
void pm_perm_sector_begin( const char* pszInfo, const char* pszFile, int line  );
void pm_perm_sector_end();
int pm_perm_item_register( const char* pszInfo, const char* pszFile, int line  );
void pm_perm_item_begin( int index );
void pm_perm_item_end( int index );
void pm_perm_get_report( ByteStr* bs );

#ifdef PM_PERFORMANCE_ANA
	#define PM_PERM_INIT	pm_perm_init()
	#define _PM_PERM_SECTOR_BEGIN( sec ) pm_perm_sector_begin( #sec, __FILE__, __LINE__ )
	#define _PM_PERM_SECTOR_END( ) pm_perm_sector_end()

	#define _PM_PERM_BEGIN( key ) {\
			static int s_##key##_index = 0;\
			if( s_##key##_index == 0 )\
				s_##key##_index = pm_perm_item_register( #key, __FILE__,__LINE__);\
			pm_perm_item_begin( s_##key##_index );
		
	#define _PM_PERM_END(key) pm_perm_item_end( s_##key##_index ); }
	#define PM_PERM_REPORT do{ ByteStr* bs = _BS(""); pm_perm_get_report( bs ); \
		pm_info1( "%s" PM_CRLF, bytestr_string(bs)); \
		bytestr_destroy(bs);}while(0)
	#define PM_PERM_REPORT1 do{ ByteStr* bs = _BS(""); pm_perm_get_report( bs ); \
		printf( "%s" PM_CRLF, bytestr_string(bs)); \
		bytestr_destroy(bs);}while(0)
#else
	#define PM_PERM_INIT 
	#define _PM_PERM_SECTOR_BEGIN( sec )
	#define _PM_PERM_SECTOR_END()
	#define _PM_PERM_BEGIN( key )
	#define _PM_PERM_END(key)
	#define PM_PERM_REPORT
	#define PM_PERM_REPORT1
#endif

#ifdef __cplusplus
}
#endif

#endif
