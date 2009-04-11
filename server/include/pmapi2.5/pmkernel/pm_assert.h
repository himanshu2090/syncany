#ifndef __PM_ASSERT_H__
#define __PM_ASSERT_H__

/** \brief	bytestr_º¯Êý

*/
#ifdef __cplusplus
extern "C"
{
#endif


void pm_assert_real_func( int bAssertValue, const char* fname, int line );
#ifdef PM_DEBUG
	#define pm_assert_real( b,f,l ) pm_assert_real_func( !!(b), f,l )
	#define pm_assert(b) pm_assert_real_func( !!(b), __FILE__, __LINE__ )
#else
	#define pm_assert_real( p1, p2, p3 )
	#define pm_assert(b)
#endif

#ifdef __cplusplus
}
#endif

#endif
