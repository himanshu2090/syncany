#ifndef __PM_PUB_FUN_H__
#define __PM_PUB_FUN_H__

#ifdef __cplusplus
extern "C"
{
#endif

char* pm_trim(char* str);
char* pm_strip_crlf(char *str);
/* 
 * Description:
 *     use the original buffer to create a char pointer array 
 * Parameter:
 *     line: the stirng
 *     argv: buffer, allocated by calling function
 *     argc: [in] argv's size, [out] argument count
 */
char** pm_str_to_argv(char* line, char** argv, int* argc);

/**	\brief	The pm_star_match function

	\param	pszm[in] a parameter of type const char* 模式串,可带星号问号
	\param	pszmd[in] a parameter of type const char* 被匹配串
	\return	int 相等为0，大于>0, 小于<0
	\note 带星号的模式匹配
*/
int pm_star_match( const char* pszm, const char* pszmd );
int pm_start_match_i( char* pszm/*模式串,可带星号冒号*/, const char* pszmd/*被匹配串*/ );//带星号的模式匹配
char* pm_trim_right(char* szSrc);
PM_BOOL pm_is_print( const char* psz, int len );
PM_BOOL pm_is_ascii( const char* psz, int len );
char* pm_remove_q( char* psz );
PM_HANDLE pm_create_apr_pool();
void pm_destroy_apr_pool( PM_HANDLE* p_pool );

const char* pm_get_temp_dir();
void pm_set_temp_dir( const char* pszTempDir );
char* pm_get_temp_filename( const char* pszPrefix, char* buf );

//#define PM_APR_POOL ((apr_pool_t*)pm_create_apr_pool())

PM_HANDLE pm_shm_create( PM_UINT nSize, const char *filename );
void pm_shm_destroy( PM_HANDLE hShm );
PM_BOOL pm_shm_remove(const char * );
void* pm_shm_addr( PM_HANDLE hShm );
PM_UINT pm_shm_get_size(PM_HANDLE hShm );


int pm_system_hfile( 
	const char** args, /*in, command*/
	PM_HANDLE hFileIn,/*in, stdin , can be PM_NULL*/
	PM_HANDLE hFileOut,/*in, stdout , can be PM_NULL*/
	PM_HANDLE hFileErr,/*in, stderr , can be PM_NULL*/
	int nTimeout/*in, timeout, use second, if 0, wait infinite*/
);
int pm_system_filename( 
	const char** args, /*in, command*/
	const char* pszFileIn,/*in, stdin , can be PM_NULL*/
	const char* pszFileOut,/*in, stdout , can be PM_NULL*/
	const char* pszFileErr,/*in, stderr , can be PM_NULL*/
	int nTimeout/*in, timeout, use second, if 0, wait infinite*/
);
int pm_system_str( 
	const char** args, /*in, command*/
	ByteStr* bsin,/*in, stdin , can be PM_NULL*/
	ByteStr* bsout,/*in, stdout , can be PM_NULL*/
	ByteStr* bserr,/*in, stderr , can be PM_NULL*/
	int nTimeout/*in, timeout, use second, if 0, wait infinite*/
);
int pm_system_str2( 
	const char** args, /*in, command*/
	ByteStr* bsin,/*in, stdin , can be PM_NULL*/
	ByteStr* bsout,/*in, stdout and stderr, can be PM_NULL*/
	int nTimeout/*in, timeout, use second, if 0, wait infinite*/
);

PM_LONG pm_inc( PM_LONG* );
PM_LONG pm_dec( PM_LONG* );

char* pm_to_upper(char* str);
char* pm_to_lower(char* str);

#ifdef __cplusplus
}
#endif


#endif
