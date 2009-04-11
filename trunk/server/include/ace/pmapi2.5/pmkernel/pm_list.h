#ifndef __PM_LIST_H__
#define __PM_LIST_H__

/**	
 *@file pm_list.h
 *@brief 
*/

//本文件函数需要仔细考虑

#ifdef UNIX
#include <pthread.h>
#endif

///节点数据结构
typedef struct pm_list_node
{
    PM_LPVOID data;		
    struct pm_list_node *prev, *next;
}PM_LIST_NODE;

///链表数据结构(双向链表)
typedef struct pm_list
{
    PM_LIST_NODE *head, *tail, *cur_node;
    PM_INT		size;		//the node total in the list

#if defined(UNIX)
    pthread_cond_t 	*nonempty;
    pthread_mutex_t	*lock;	
    PM_INT alive;
    PM_INT num_locks;
#endif

}PM_LIST;

/** \brief pmlist_DATA_PACKER函数指针

*/
typedef ByteStr *(* pmlist_DATA_PACKER)(PM_LPVOID);
/** \brief pmlist_DATA_PACKER函数指针

*/
typedef PM_LPVOID(* pmlist_DATA_UNPACKER) (PM_LPVOID);
/** \brief pmlist_DATA_PACKER函数指针

*/
typedef PM_INT (pmlist_DATA_COMPARER) (PM_LPVOID, PM_LPVOID);

#ifdef __cplusplus
extern "C" {
#endif
/** \brief pmlist_create

*/
PM_LIST *pmlist_create();
/** \brief pmlist_destroy_real

*/
PM_VOID pmlist_destroy_real ( PM_LIST *l );
/** \brief pmlist_rewind

*/
PM_VOID pmlist_rewind ( PM_LIST *xml );
/** \brief pmlist_at

*/
PM_LPVOID pmlist_at ( PM_LIST *l, PM_INT index );
/** \brief pmlist_push_back

*/
PM_LIST_NODE *pmlist_push_back(PM_LIST *l, PM_LPVOID data);
/** \brief pmlist_insert

*/
PM_LIST_NODE *pmlist_insert( PM_LIST *l, PM_LIST_NODE *node, PM_LPVOID data);
/** \brief pmlist_push_head

*/
PM_LIST_NODE *pmlist_push_head(PM_LIST *l, PM_LPVOID data);
/** \brief pmlist_remove_node

*/
PM_VOID pmlist_remove_node(PM_LIST *l, PM_LIST_NODE *node);
/** \brief pmlist_pop_first

*/
PM_LPVOID pmlist_pop_first(PM_LIST *el);
/** \brief pmlist_next

*/
PM_LPVOID pmlist_next(PM_LIST *el);
/** \brief pmlist_prev

*/
PM_LPVOID pmlist_prev(PM_LIST *el);
/** \brief pmlist_swap

*/
PM_INT pmlist_swap(PM_LIST *el, PM_INT lo, PM_INT hi);
/** \brief pmlist_sort

*/
PM_VOID pmlist_sort(PM_LIST *a, pmlist_DATA_COMPARER *compf);

#if  defined(UNIX)

PM_INT pmlist_lock(PM_LIST *el);
PM_VOID pmlist_unlock(PM_LIST *el);
PM_VOID pmlist_signal(PM_LIST *el);
PM_VOID pmlist_release(PM_LIST *el);
PM_LIST_NODE *pmlist_sync_append(PM_LIST *el, PM_LPVOID data);
PM_LPVOID pmlist_sync_pop_first(PM_LIST *el);

#endif
/** \brief pmlist_size

*/
PM_INT pmlist_size(PM_LIST *el);
/** \brief pmlist_find

*/
PM_LIST_NODE *pmlist_find(PM_LIST *el, PM_LPVOID data);
/** \brief pmlist_find_ex

*/
PM_LPVOID pmlist_find_ex(PM_LIST *el, PM_LPVOID tar, PM_INT (*cmpfunc)(PM_LPVOID src, PM_LPVOID tar));
/** \brief pmlist_destroy_ex

*/
PM_VOID pmlist_destroy_ex(PM_LIST *el, PM_VOID (*func)(PM_LPVOID  data));
/** \brief pmlist_remove_ex

*/
PM_VOID pmlist_remove_ex(PM_LIST *el, PM_LPVOID data, PM_VOID (*func)(PM_LPVOID d));
/** \brief pmlist_unpack

*/
//PM_LIST *pmlist_unpack(ByteStr *pack, PM_LPVOID (*func)(ByteStr *));
/** \brief pmlist_pack

*/
//ByteStr *pmlist_pack(PM_LIST *el, ByteStr *(*func)(PM_LPVOID ));
/** \brief bytestr_split

*/

/** \brief pmlist_dump

*/
PM_INT pmlist_dump( PM_LIST *el, PM_LPCSTR filename, pmlist_DATA_PACKER pack_func );

#ifdef __cplusplus
}
#endif
/** \brief pmlist_append

*/
#define pmlist_append pmlist_push_back

/** \brief pmlist_destroy

*/
#define pmlist_destroy(el) do{ pmlist_destroy_real(el); el = PM_NULL; }while(0)

//#define pmlist_destroy(el) do{ pmlist_destroy_real(el); el = NULL; }while(0)


////////////////////////////////////////////////////////////////////////////////
#endif
