#ifndef __PM_MEMORY_CACHED__H__
#define __PM_MEMORY_CACHED__H__

typedef PM_HANDLE PM_HMEMCACHE;
typedef PM_HANDLE PM_HMEMCACHE_SERVER_ST;
PM_HMEMCACHE pm_mem_cache_create();
int pm_mem_cache_server_push( PM_HMEMCACHE h, PM_HMEMCACHE_SERVER_ST st );
PM_HMEMCACHE_SERVER_ST pm_mem_cache_servers_parse( char* psz );
void pm_mem_cache_server_list_free( PM_HMEMCACHE_SERVER_ST st );
void pm_mem_cache_free( PM_HMEMCACHE h );
PM_HMEMCACHE pm_mem_cache_clone( PM_HMEMCACHE h1,PM_HMEMCACHE h2 );
char *pm_mem_cache_get( PM_HMEMCACHE h, const char* key, PM_UINT key_length, PM_UINT *value_length,  PM_UINT* flags, int* error ) ;
#endif

