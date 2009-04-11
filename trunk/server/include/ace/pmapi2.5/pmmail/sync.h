#ifndef __SYNC__H__
#define __SYNC__H__

#define SYNC_ACTION_ADD    1
#define SYNC_ACTION_DEL    2
#define SYNC_ACTION_MODIFY 3
#include "sync_pdu.h"

/**	\brief The Sync struct
*/
typedef struct 
{
    ByteStr *id;
    ByteStr *userid;
    PM_ULONG  seedformat;
    long    state;
    int     meccid;
    PM_LIST  *items;

    /* used only by server side, and won't be packed in package */
    ByteStr *host;
}Sync, *SyncPtr;


#ifdef __cplusplus 
extern "C" {
#endif

int CodeConvert(ByteStr *uni_data, const char *from, const char *to);
void SyncDestroy(SyncPtr sync);
void SyncAddPdu(SyncPtr sync, SyncPduPtr pdu);
void SyncDump(SyncPtr syncP, const char *, int);
void hton_unicode(ByteStr *uni_data);
SyncPtr SyncCreate();
ByteStr * SyncPack(Sync *sync);
ByteStr * SyncPackPlain(Sync *sync, int *is_unicode);
Sync * SyncUnpack(ByteStr *pack);


//int run_script(ByteStr *script_content, ByteStr *ret);
int run_command(ByteStr *cmd, ByteStr *ret);
PM_BOOL truncate_mail_address_list(ByteStr *addr_list, PM_ULONG max_len);
//PM_BOOL read_until_dot(ByteStr *msgdata, FILE *fp);
PM_BOOL is_charset_valid(int i);
PM_BOOL change_file_owner(PM_LPCSTR path, PM_LPCSTR owner);
PM_BOOL filter_match(PM_LPCSTR src, PM_LPCSTR pattern);
PM_BOOL verify_mail_address(PM_LPCSTR emailaddress);
//PM_LPCSTR get_charset(int charset_index);
//PM_LPCSTR make_uid(char *uid, int size);



SyncPdu* refined_mlib_parse_file_ex(const char *mailpath, const char *wanted_charset, int strip_att_content);
SyncPdu* refined_mlib_parse_memory_ex(ByteStr *mail_memory, const char *wanted_charset, int strip_att_content);
eMail_Attachment* refined_mlib_get_attachment(const char *mailpath, int desired_part);
eMail_Attachment* refined_mlib_get_attachment_m(ByteStr *mailbuf, int desired_partno);
char *refined_mlib_get_boundary(const char *mailpath, char *boundary_buffer, int size);
char *refined_mlib_get_boundary_m(ByteStr *mail_source, char *boundary_buffer, int size);
SyncPdu* refined_mlib_parse_file(const char* mailpath);
SyncPdu* refined_mlib_parse_memory(ByteStr* mail_memory);

#if 0
	SyncPdu* mlib_parse_file_ex(const char *mailpath, const char *wanted_charset, int strip_att_content);
	SyncPdu* mlib_parse_memory_ex(ByteStr *mail_memory, const char *wanted_charset, int strip_att_content);
	eMail_Attachment* mlib_get_attachment(ByteStr *mailbuf, int desired_part);
	char *mail_get_boundary(const char *mailpath, char *boundary_buffer, int size);
	char *mail_get_boundary_m(ByteStr *mail_source, char *boundary_buffer, int size);
	SyncPdu* mlib_parse_file(const char* mailpath);
	SyncPdu* mlib_parse_memory(ByteStr* mail_memory);

	#define mlib_get_boundary	mail_get_boundary

#else

	#define mlib_parse_file_ex	refined_mlib_parse_file_ex
	#define mlib_parse_memory_ex refined_mlib_parse_memory_ex	
	#define mlib_get_attachment	refined_mlib_get_attachment
	#define mlib_get_boundary	refined_mlib_get_boundary
	#define mlib_parse_file	refined_mlib_parse_file
	#define mlib_parse_memory refined_mlib_parse_memory

#endif

#define mlib_get_boundary_m	refined_mlib_get_boundary_m
#define mlib_get_attachment_m	refined_mlib_get_attachment_m


void dump_mail(SyncPdu *pdu);



#ifdef __cplusplus 
}
#endif

#ifdef WIN32
	#ifndef PMMAIL_EXPORTS
		#ifdef _DEBUG
			#pragma comment(lib,"pmmailD.lib")
		#else
			#pragma comment(lib,"pmmail.lib")
		#endif
	#endif
#endif
#endif
