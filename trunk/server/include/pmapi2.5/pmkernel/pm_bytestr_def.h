#ifndef __PM_BYTESTR_DEF__
#define __PM_BYTESTR_DEF__
//次定义只允许pm_bytestr.c和pm_bytestr_charset.c里引用

#define DEFINED_BYTESTR
typedef struct pm_bytestr
{
	PM_BYTE* data;		///<data buffer
	PM_ULONG len;		///< real data length, it is smaller than size at least 1, I leave 1 PM_BYTE for '\\0' in case it is a string 
	PM_ULONG size;		///< the real memory space size of data, it is increased doublely to prevent memory allocation frequently 
	PM_INT lock;		///<锁
	PM_INT charset;		///<字符集索引值
	PM_ULONG cur_pos;	///<当前指针位置
	PM_INT	ref;		///当前引用成员data的ByteStr结构，肖保锋加(非线程安全)
//	void* reg_list;
	PM_ULONG extra;		///< it is used for special app, which wants to represent the data with extra information 
}ByteStr;

#endif

