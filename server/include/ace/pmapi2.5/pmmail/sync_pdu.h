#ifndef __SYNC__PDU__
#define __SYNC__PDU__

/**	
 *@file sync_pdu.h 
 *@brief ¿Ú
*/


/**	\brief PDU
*/
#define PDU(name, data) \
    sync_pdu_type_##name,

enum {
    sync_pdu_type_None=0,
    #include "sync_pdu.def"
    sync_pdu_type_Total
};
/**	\brief TYPE
*/
#define TYPE(name, members) \
	type_id_##name,
enum{
    type_id_Zero=0,
    #include "sync_pdu.def"
    type_id_Total
};
/**	\brief INTEGER
*/
#define INTEGER(data, len)	PM_ULONG data;
/**	\brief OCTETS
*/
#define OCTETS(data)  		ByteStr *data;
/**	\brief NULTERMINATED
*/
#define NULTERMINATED(data)  	ByteStr *data;
/**	\brief STRLIST
*/
#define STRLIST(data)  		PM_LIST  *data;
/**	\brief TYPEDLIST
*/
#define TYPEDLIST(data, type)	PM_LIST  *data;
/**	\brief PDU
*/
#define PDU(name, data) \
    typedef struct name data name;
/**	\brief TYPE
*/
#define TYPE(name, members) \
    typedef struct name members name;
#include "sync_pdu.def"

/**	\brief	The SyncPdu struct

*/
typedef struct{
	int	pdu_type;
	int	action;

	union{
		#define PDU(name, data) \
			struct name	name;
#include "sync_pdu.def"
	}u;
}SyncPdu, *SyncPduPtr;/**<SyncPdu */


#ifdef __cplusplus 
extern "C" {
#endif

/**	\brief	The sync_pdu_create function

	\param	pdu_type	a parameter of type int

	\return	SyncPduPtr

	
*/
SyncPduPtr sync_pdu_create(int pdu_type);
void sync_pdu_destroy(SyncPduPtr pdu);
/**	\brief	The sync_pdu_dump function

	\param	pdu	a parameter of type SyncPduPtr

	\return	void

	
*/
void sync_pdu_dump(SyncPduPtr pdu);
//ByteStr *sync_pdu_pack(SyncPduPtr pdu);
//SyncPduPtr sync_pdu_unpack(ByteStr *bs);
/**	\brief	The sync_pdu_pack function

	\param	packed_data	a parameter of type ByteStr *
	\param	pdu	a parameter of type SyncPduPtr

	\return	int

	
*/
int sync_pdu_pack(ByteStr *packed_data, SyncPduPtr pdu);
/**	\brief	The sync_pdu_unpack function

	\param	packed_data	a parameter of type ByteStr *
	\param	posPtr	a parameter of type PM_ULONG *

	\return	SyncPduPtr

	
*/
SyncPduPtr sync_pdu_unpack(ByteStr *packed_data, PM_ULONG *posPtr);
/**	\brief	TYPE

*/
#define TYPE(name, members) \
	ByteStr *Pack##name(name *varPtr); \
	name *Unpack##name(ByteStr *packed_data); \
	void Destroy##name(name *varPtr);
#include "sync_pdu.def"
/**	\brief	The *type_create function

	\param	type_id	a parameter of type int

	\return	void

	
*/
void *type_create(int type_id);
/**	\brief	The type_destroy function

	\param	type_id	a parameter of type int
	\param	varPtr	a parameter of type void *

	\return	void

	
*/
void type_destroy(int type_id, void *varPtr);

/**	\brief	The sync_pdu_test function


	\return	void

	
*/
void sync_pdu_test();



#ifdef __cplusplus 
}
#endif

#endif
