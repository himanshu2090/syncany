#ifndef __PM_BYTESTR_DEF__
#define __PM_BYTESTR_DEF__
//�ζ���ֻ����pm_bytestr.c��pm_bytestr_charset.c������

#define DEFINED_BYTESTR
typedef struct pm_bytestr
{
	PM_BYTE* data;		///<data buffer
	PM_ULONG len;		///< real data length, it is smaller than size at least 1, I leave 1 PM_BYTE for '\\0' in case it is a string 
	PM_ULONG size;		///< the real memory space size of data, it is increased doublely to prevent memory allocation frequently 
	PM_INT lock;		///<��
	PM_INT charset;		///<�ַ�������ֵ
	PM_ULONG cur_pos;	///<��ǰָ��λ��
	PM_INT	ref;		///��ǰ���ó�Աdata��ByteStr�ṹ��Ф�����(���̰߳�ȫ)
//	void* reg_list;
	PM_ULONG extra;		///< it is used for special app, which wants to represent the data with extra information 
}ByteStr;

#endif

