#ifndef __BYTESTR_H__
#define __BYTESTR_H__

/**	@file pm_bytestr.h
	@brief bytestr_函数.

*/

/**	\brief MAX_BYTESTR_SIZE
*/
#define MAX_BYTESTR_SIZE ((PM_ULONG)50*1024*1024)   // 50M

#ifdef __cplusplus
extern "C"
{
#endif

/**	\brief	The bytestr_at_real function

	\param	bs	a parameter of type ByteStr*
	\param	pos	a parameter of type PM_ULONG

	\return	PM_BYTE

	
*/
		PM_BYTE bytestr_at_real(ByteStr* bs, PM_ULONG pos);
/** \brief bytestr_at

*/
		#define bytestr_at bytestr_at_real
	//#endif

/** \brief _BS

*/
	#define _BS      bytestr_create
/** \brief bs_cstr(s)

*/
	#define bs_cstr(s) ((((ByteStr*)s)==PM_NULL) ? "" :  bytestr_string(s))
/** \brief bs_len

*/
	#define bs_len   bytestr_length
/** \brief bs_data

*/
	#define bs_data  bytestr_data


/**	\brief	在ByteStr后附加一个PM_LONG的值

	\param	bs	a parameter of type ByteStr*
	\param	lValue	a parameter of type PM_LONG

	\return	PM_VOID
	
*/
	PM_VOID bytestr_append_decimal(ByteStr* bs, PM_LONG lValue);


/**	\brief	把格式化字符串输出到ByteStr中

	\param	bs	a parameter of type ByteStr*
	\param	fmt	a parameter of type PM_LPCSTR
	\param	args	a parameter of type PM_ARGLIST

	\return	PM_VOID

	
*/
	PM_VOID bytestr_format_valist(ByteStr* bs, PM_LPCSTR fmt, PM_ARGLIST args);

	///把格式化字符串输出到ByteStr中
/**	\brief	把格式化字符串输出到ByteStr中

	\param	bs	a parameter of type ByteStr*
	\param	format	a parameter of type PM_LPCSTR

	\return	PM_VOID

	
*/
	PM_VOID bytestr_format(ByteStr* bs, PM_LPCSTR format, ...);

	///把格式化字符串添加到ByteStr中
/**	\brief	把格式化字符串添加到ByteStr中

	\param	bs	a parameter of type ByteStr*
	\param	fmt	a parameter of type PM_LPCSTR

	\return	PM_VOID

	
*/
	PM_VOID bytestr_format_append(ByteStr* bs, PM_LPCSTR fmt, ...);

	///对ByteStr进行url编码
/**	\brief	对ByteStr进行url编码

	\param	bs	a parameter of type ByteStr*

	\return	PM_VOID

	
*/
	PM_VOID bytestr_url_encode(ByteStr* bs);


/**	\brief	对ByteStr进行url解码

	\param	bs	a parameter of type ByteStr*

	\return	PM_INT

	
*/
	PM_INT bytestr_url_decode(ByteStr* bs);


/**	\brief	对ByteStr进行url编码后输出

	\param	bs	a parameter of type ByteStr*
	\param	fp	a parameter of type PM_STREAM *

	\return	PM_INT

	
*/
//	PM_INT bytestr_url_dump(ByteStr* bs, PM_STREAM * fp);	 


/**	\brief	对ByteStr加锁
	\param	bs	a parameter of type ByteStr*

	\return	PM_VOID

	
*/
	PM_VOID bytestr_lock(ByteStr* bs);


/**	\brief	解锁

	\param	bs	a parameter of type ByteStr*

	\return	PM_VOID

	
*/
	PM_VOID bytestr_unlock(ByteStr* bs);


/**	\brief	数据拷贝到ByteStr中

	\param	bs	a parameter of type ByteStr*
	\param	data	a parameter of type PM_BYTE*
	\param	len	a parameter of type PM_ULONG

	\return	PM_VOID

	
*/
	PM_VOID bytestr_ncopy(ByteStr* bs, PM_BYTE* data, PM_ULONG len);


/**	\brief	设置ByteStr缺省缓冲区大小

	\param	size	a parameter of type PM_INT

	\return	PM_VOID

	
*/
	PM_VOID bytestr_set_default_size(PM_INT size);


	/**	\brief	bytestr_destroy_real 释放ByteStr
		\param[in]	bs	a parameter of type ByteStr*
		\param[in]	callfile	调用该函数的文件
		\param[in]	line	调用处的行号
	*/
	int bytestr_destroy_real(ByteStr* bs, PM_LPCSTR callfile, PM_INT line);


/**	\brief	把ByteStr的长度截成len指定大小(len要小于BytsStr的实际长度)

	\param	bs	a parameter of type ByteStr*
	\param	len	a parameter of type PM_ULONG

	\return	PM_VOID

	
*/
	PM_VOID bytestr_truncate(ByteStr* bs, PM_ULONG len);


/**	\brief	去掉ByteStr中指定位置，指定长度的内容(后面没有消除的内容前移)

	\param	bs	a parameter of type ByteStr*
	\param	pos	a parameter of type PM_ULONG
	\param	len	a parameter of type PM_ULONG

	\return	PM_VOID

	
*/
	PM_VOID bytestr_remove(ByteStr* bs, PM_ULONG pos, PM_ULONG len);

/**	\brief	trim a string(包括' ','\\r','\\n','\\t')

	\param	bs	a parameter of type ByteStr*

	\return	PM_VOID

	
*/
	PM_VOID bytestr_trim(ByteStr* bs);


/**	\brief	清空

	\param	bs	a parameter of type ByteStr*

	\return	PM_VOID

	
*/
	PM_VOID bytestr_empty(ByteStr* bs);


/**	\brief	搜索html标记('<','>'),并把ByteStr该为标记中间的内容

	\param	bs	a parameter of type ByteStr*

	\return	PM_VOID

	
*/
	PM_VOID bytestr_strip_html_tag(ByteStr* bs);


/**	\brief	从二进制转换成十六进制

	\param	bs	a parameter of type ByteStr*
	\param	uppercase	a parameter of type PM_INT

	\return	PM_VOID

	
*/
	PM_VOID bytestr_binary_to_hex(ByteStr* bs, PM_INT uppercase);


/**	\brief	从十六进制转换成二进制，Return 1 on success, 0 on error

	\param	bs	a parameter of type ByteStr*

	\return	PM_INT

	
*/
	PM_INT bytestr_hex_to_binary(ByteStr* bs);



/**	\brief	trim a str('\\r','\\n')

	\param	bs	a parameter of type ByteStr*

	\return	PM_VOID

	
*/
	PM_VOID bytestr_strip_crlf(ByteStr* bs);

/**	\brief	将输入普通的字符串转为base64

	\param	bs	a parameter of type ByteStr*

	\return	ByteStr*

	
*/
	ByteStr* bytestr_base64_encode(ByteStr* bs);

/**	\brief	将base64的字符串转为普通字符串

	\param	bs	a parameter of type ByteStr*

	\return	ByteStr*

	
*/
	ByteStr* bytestr_base64_decode(ByteStr* bs);

/**	\brief	复制字符集 （clone）

	\param	bs	a parameter of type ByteStr*
	\param	f	a parameter of type PM_LPCSTR
	\param	l	a parameter of type PM_INT

	\return	ByteStr*

	
*/
	ByteStr* bytestr_clone_real(ByteStr* bs, PM_LPCSTR f, PM_INT l);

	/**	\brief	根据指定的长度将data数据创建类型为ByteStr的字符串
		\param[in]	data	data buffer
		\param[in]	size    data len
		\return	success: return ByteStr* ,or return PM_NULL
	*/
	ByteStr* bytestr_create_from_data(PM_LPVOID data, PM_ULONG size);


/**	\brief	根据const char *类型的字符串创建类型为ByteStr的字符串

	\param	s	a parameter of type PM_LPCSTR

	\return	ByteStr*
	
*/
	ByteStr* bytestr_create(PM_LPCSTR s);


/**	\brief	根据指定的长度创建类型为ByteStr的字符串

	\param	size	a parameter of type PM_ULONG

	\return	ByteStr*指针
	
*/
	ByteStr* bytestr_create_buffer_real(PM_ULONG size, PM_LPCSTR, PM_INT);

	
/**	\brief	string to ByteStr

	\param	bs	a parameter of type ByteStr*
	\param	s	a parameter of type PM_LPCSTR

	\return	ByteStr*

	
*/
	ByteStr* bytestr_strcpy(ByteStr* bs, PM_LPCSTR s);


/**	\brief	cat a string to ByteStr

	\param	bs	a parameter of type ByteStr*
	\param	s	a parameter of type PM_LPCSTR

	\return	ByteStr*

	
*/
	ByteStr* bytestr_strcat(ByteStr* bs, PM_LPCSTR s);


/**	\brief	cat data to ByteStr

	\param	bs	a parameter of type ByteStr*
	\param	s	a parameter of type const PM_BYTE*
	\param	n	a parameter of type PM_INT

	\return	ByteStr*

	
*/
	ByteStr* bytestr_strncat(ByteStr* bs, const PM_BYTE* s, PM_INT n);


/**	\brief	copy data to ByteStr according to specified length

	\param	bs	destination string
	\param	s	source string
	\param	n	length

	\return	ByteStr*

	
*/
	ByteStr* bytestr_copy_data(ByteStr* bs, const PM_BYTE* s, PM_INT n);

/**	\brief	append a long value to ByteStr

	\param	bs	a parameter of type ByteStr*
	\param	val	source string

	\return	ByteStr*
	
*/
	ByteStr* bytestr_append_long(ByteStr* bs, PM_LONG val);


	/**	\brief	bytestr_extract_real 获取ByteStr中指定位置、指定长度的数据
		\param	bs	要提取的目标ByteStr
		\param	pos	提取位置
		\param	len	提取长度
		\param	filename	调用处的文件名称
		\param	lineno	调用处的行号
		\return	若成功则返回提取的新ByteStr，否则返回PM_NULL
	*/
	ByteStr* bytestr_extract_real(ByteStr* bs, PM_ULONG pos, PM_ULONG len,
								PM_LPCSTR filename, PM_INT lineno);

/**	\brief	将bs2字符串加到bs1后面

	\param	bs1	a parameter of type ByteStr*
	\param	bs2	a parameter of type ByteStr*
	\param	filename	a parameter of type PM_LPCSTR
	\param	line	a parameter of type int

	\return	ByteStr*

	
*/
	ByteStr* bytestr_join_real(ByteStr* bs1, ByteStr* bs2, PM_LPCSTR filename, int line);

/**	\brief	create a ByteStr from file 

	\param	filename	a parameter of type PM_LPCSTR

	\return	ByteStr*

	
*/
	ByteStr* bytestr_create_from_file(PM_LPCSTR filename);

/**	\brief	在指定位置处获取字符串的实际操作

	\param	bs	a parameter of type ByteStr*
	\param	pos	a parameter of type PM_ULONG
	\param	filename	a parameter of type PM_LPCSTR
	\param	lineno	a parameter of type PM_INT

	\return	ByteStr*

	
*/
	ByteStr* bytestr_extract_string_real(ByteStr* bs, PM_ULONG pos,
									   PM_LPCSTR filename, PM_INT lineno);

/**	\brief	把bs中的src串替换成tar串

	\param	bs	a parameter of type ByteStr*
	\param	src	a parameter of type PM_LPCSTR
	\param	tar	a parameter of type PM_LPCSTR

	\return	ByteStr*

	
*/
	ByteStr* bytestr_replace_sub_string(ByteStr* bs, PM_LPCSTR src,
									  PM_LPCSTR tar);


/** \brief bytestr_append_read_real

*/
	PM_INT bytestr_append_read_real(ByteStr* bs, PM_INT fd, PM_ULONG length,
							   PM_LPCSTR filename, PM_INT lineno);	//==================
/** \brief bytestr_read_real

*/
	PM_INT bytestr_read_real(ByteStr* bs, PM_INT fd, PM_ULONG length,
						PM_LPCSTR filename, PM_INT lineno);	  //==================
/** \brief bytestr_nwrite

*/
	PM_INT bytestr_nwrite(ByteStr* bs, PM_ULONG pos, PM_ULONG length, PM_INT fd); //==================

	/**	\brief	bytestr_writefile 把ByteStr的内容写入到文件中
		\param	bs	目标ByteStr
		\param	filename	文件名称
		\return	success：0；error：－1	
	*/
	PM_INT bytestr_writefile(ByteStr* bs, PM_LPCSTR filename);


/**	\brief	设置ByteStr的数据长度(len要小于ByteStr的缓冲区大小，否则返回-1)

	\param	bs	a parameter of type ByteStr*
	\param	len	a parameter of type PM_ULONG

	\return	PM_INT

	
*/
	PM_INT bytestr_set_length(ByteStr* bs, PM_ULONG len);


/**	\brief	返回缓冲区大小

	\param	bs	a parameter of type ByteStr*

	\return	PM_INT

	
*/
	PM_INT bytestr_size(ByteStr* bs);


/**	\brief	在指定位置插入长度为len的数据，成功返回0，否则返回1；

	\param	bs	a parameter of type ByteStr*
	\param	pos	a parameter of type PM_ULONG
	\param	data	a parameter of type PM_BYTE*
	\param	len	a parameter of type PM_ULONG

	\return	PM_INT

	
*/
	PM_INT bytestr_insert(ByteStr* bs, PM_ULONG pos, PM_BYTE* data, PM_ULONG len);


/**	\brief	compare two ByteStr return 1,0,-1

	\param	bs1	a parameter of type ByteStr*
	\param	bs2	a parameter of type ByteStr*

	\return	PM_INT

	
*/
	PM_INT bytestr_compare(ByteStr* bs1, ByteStr* bs2);


/**	\brief	重新设置ByteStr缓冲区大小，成功返回1， 失败返回0

	\param	bs	a parameter of type ByteStr*
	\param	size	a parameter of type PM_ULONG

	\return	PM_INT

	
*/
	PM_INT bytestr_resize(ByteStr* bs, PM_ULONG size);


PM_INT bytestr_freadline(ByteStr* bs, PM_HANDLE hFile);

/**	\brief	append a char to bs

	\param	bs	a parameter of type ByteStr*
	\param	c	a parameter of type PM_CHAR

	\return	PM_INT
	
*/
	PM_INT bytestr_append_char(ByteStr* bs, PM_CHAR c);


/**	\brief	设置ByteStr结构中extra属性的值

	\param	bs	a parameter of type ByteStr*
	\param	d	a parameter of type PM_ULONG

	\return	PM_VOID

	
*/
	PM_VOID bytestr_set_extra(ByteStr* bs, PM_ULONG d);


/**	\brief	获取ByteStr结构中extra属性的值

	\param	bs	a parameter of type ByteStr*

	\return	PM_ULONG
	
*/
	PM_ULONG bytestr_get_extra(ByteStr* bs);

/**	\brief	read line by invoking recv, used to read data from socket on windows 

	\param	bs	a parameter of type ByteStr*
	\param	fd	a parameter of type PM_INT

	\return	PM_INT

	
*/
	PM_INT bytestr_recvline(ByteStr* bs, PM_INT fd);	//==================

/**	\brief	read the whole file into bs, when success, return the size read ,or return -1

	\param	bs	a parameter of type ByteStr*
	\param	filename	a parameter of type PM_LPCSTR

	\return	PM_INT

	
*/
	PM_INT bytestr_readfile(ByteStr* bs, PM_LPCSTR filename);


	/**	\brief	bytestr_fread read data into buffer
		\param[in,out]	bs 返回的目标值 	 
		\param[in]	fp	源文件句柄
		\param[in]	size 要读取的大小
		\return	成功：返回实际读取的大小；失败：-1表示读取失败；-2表示size过大	
	*/
	PM_INT bytestr_fread(ByteStr* bs, PM_HANDLE fp, PM_ULONG size);


	/**	\brief	bytestr_fwrite 把内容写入到文件中
		\param	bs	源内容
		\param	fp	文件句柄
		\param	size 写入的大小	 
		\return	若成功则返回写入的实际大小，失败返回PM_ERROR	
	*/
	PM_INT bytestr_fwrite(ByteStr *bs, PM_HANDLE fp, PM_ULONG size);


	///compare bs1 and cstr,return 1, 0, or -1
	PM_INT bytestr_str_compare(ByteStr* bs1, PM_LPCSTR cstr);

	///find a str,success return the position(based 0),or return -1 
	PM_INT bytestr_find_string(ByteStr* bs, PM_LPCSTR str);

	///find a char,success return the position(based 0),or return -1
	PM_INT bytestr_find_char(ByteStr* bs, PM_BYTE c);

	///用val替代指定位置的值，0 on success, -1 on error
	PM_INT bytestr_replace_short(ByteStr* bs, PM_ULONG pos, short val);

/** \brief bytestr_dump_real

*/
	PM_INT bytestr_dump_real(ByteStr* bs, PM_LPCSTR, PM_LPCSTR file, PM_INT line);	//==================


	///swap the members of bs and bs2  
	PM_INT bytestr_swap(ByteStr* bs1, ByteStr* bs2);



	/**	\brief	bytestr_get_md5 get md5 value 
		\param[in]	bs	source ByteStr
		\param[out]	md5	returned md5 sum
		\return	0 on success, it should be always success
		\note parameter md5's buffer size must be equal to or longer than 33 
	*/
	PM_INT bytestr_get_md5(ByteStr* bs, PM_CHAR* md5);

	/**	\brief	bytestr_get_md5 get md5 value 
		\param[in]	bs	source ByteStr
		\param[out]	md5	returned md5 sum
		\param[in] n size of bytes to get md5
		\return	0 on success, -1 on error
		\note parameter md5's buffer size must be equal to or longer than 33 
	*/
	PM_INT bytestr_nget_md5(ByteStr* bs, PM_CHAR* md5, PM_ULONG n);


	///convert to long
	PM_LONG bytestr_atol(ByteStr* bs);

	///return the length of PM_BYTE stream
	PM_ULONG bytestr_length_real(const ByteStr* bs, PM_LPCSTR filename,
							 PM_INT lineno);

	///regard the data in bs as a string, append a '\\0' at the end of bs->data  and return it
	PM_LPCSTR bytestr_string(const ByteStr* bs); 


	//宏定义============================================================================
/** \brief bytestr_clone

*/
	#define bytestr_clone(a) \
		bytestr_clone_real(a, __FILE__, __LINE__)

/** \brief bytestr_append_read

*/
	#define bytestr_append_read(bs, fd, length) \
			bytestr_append_read_real(bs, fd, length, __FILE__, __LINE__)
/** \brief bytestr_read

*/
	#define bytestr_read(bs, fd, length) \
			bytestr_read_real(bs, fd, length, __FILE__, __LINE__)
/** \brief bytestr_length

*/
	#define bytestr_length(bs) bytestr_length_real( bs, __FILE__, __LINE__ )
/** \brief bytestr_create_buffer

*/
	#define bytestr_create_buffer(s) \
	  bytestr_create_buffer_real(s, __FILE__, __LINE__)
/** \brief bytestr_join

*/
	#define bytestr_join(s1, s2) \
	  bytestr_join_real(s1,s2,__FILE__,__LINE__)
/** \brief bytestr_data

*/
	  PM_BYTE* bytestr_data(ByteStr* bs);

/** \brief bytestr_extract

*/
	#define bytestr_extract(bs, pos, size ) \
			bytestr_extract_real(bs, pos, size, __FILE__, __LINE__)
/** \brief bytestr_get_long

*/
	#define bytestr_get_long(bs,pos, retval ) \
			bytestr_get_long_real(bs, pos, retval, __FILE__, __LINE__ )
/** \brief bytestr_destroy

*/
	#define bytestr_destroy(bs) bytestr_destroy_real((bs), __FILE__, __LINE__)


/** \brief bytestr_get_short

*/
	#define bytestr_get_short(bs, pos, retval) bytestr_get_short_real(bs, pos, retval, __FILE__, __LINE__)
/** \brief bytestr_extract_string

*/
	#define bytestr_extract_string(bs, pos) \
			bytestr_extract_string_real(bs, pos, __FILE__, __LINE__)

	/// append a 4-bytes PM_LONG value  
	#define bytestr_dump_with_indent(s, i) bytestr_dump_real(s, i, __FILE__, __LINE__)
/** \brief bytestr_dump

*/
	#define bytestr_dump(s) bytestr_dump_real(s, "", __FILE__, __LINE__)
		//把bs里没有用到的数据先用0填充一道,liubin
		/// bytestr_fill_remain_zero
		void bytestr_fill_remain_zero( ByteStr* bs );


/// bytestr_quoted_print_decode
	ByteStr* bytestr_quoted_print_decode(ByteStr* bs); //============有问题===============

/** \brief bytestr_duplicate

*/
#define bytestr_duplicate bytestr_clone
/** \brief bytestr_append_data

*/
#define bytestr_append_data bytestr_strncat
/** \brief bytestr_get_byte

*/
#define bytestr_get_byte(bs, pos) bytestr_at(bs, pos)
/** \brief bytestr_insert_byte

*/
#define bytestr_insert_byte(bs, pos, b) \
  do { \
    PM_BYTE tmpstr[1]; \
    tmpstr[0] = (b); \
    bytestr_insert((bs), (pos), tmpstr, 1); \
  }while(0)
/** \brief bytestr_append_byte

*/
#define bytestr_append_byte(bs, b) \
  do{ \
    PM_BYTE tmpstr[1]; \
    tmpstr[0] = (b); \
    bytestr_strncat((bs), tmpstr, 1); \
  }while(0)
/** \brief bytestr_append_short

*/
#define bytestr_append_short(bs, s) \
  do{ \
    PM_BYTE tmpstr[2]; \
    tmpstr[0] = (((s)>>8)&0xFF); tmpstr[1] = ((s)&0xFF);\
    bytestr_strncat((bs), tmpstr, 2); \
  }while(0)
/** \brief bytestr_append

*/
#define bytestr_append bytestr_join
#define PM_SAFE_STR( pszstr ) ((pszstr) == PM_NULL?"NULL":(pszstr))

///查找指定字符集的索引值，index on success, -1 if charset is not supported
PM_INT bytestr_charset_index(PM_LPCSTR charset);

///返回指定索引的字符集名称(失败返回PM_NULL)
PM_LPCSTR bytestr_charset_name(PM_INT charset_index);

int bytestr_copy( ByteStr* bsfrom, ByteStr* bsto );

void bytestr_addref( ByteStr* bs );

//所有支持的字符集
#define PM_CHARSET_US_ASCII				"US-ASCII"
#define PM_CHARSET_UTF8					"UTF-8"
#define PM_CHARSET_UCS_2				"UCS-2"
#define PM_CHARSET_UCS_2BE				"UCS-2BE"
#define PM_CHARSET_UCS_2LE				"UCS-2LE"
#define PM_CHARSET_UCS_4				"UCS-4"
#define PM_CHARSET_UCS_4BE				"UCS-4BE"
#define PM_CHARSET_UCS_4LE				"UCS-4LE"
#define PM_CHARSET_UTF_16				"UTF-16"
#define PM_CHARSET_UTF_16BE				"UTF-16BE"
#define PM_CHARSET_UTF_16LE				"UTF-16LE"
#define PM_CHARSET_UTF_32				"UTF-32"
#define PM_CHARSET_UTF_32BE				"UTF-32BE"
#define PM_CHARSET_UTF_32LE				"UTF-32LE"
#define PM_CHARSET_UTF_7				"UTF-7"
#define PM_CHARSET_UCS_2_INTERNAL		"UCS-2-INTERNAL"
#define PM_CHARSET_UCS_2_SWAPPED		"UCS-2-SWAPPED"
#define PM_CHARSET_UCS_4_INTERNAL		"UCS-4-INTERNAL"
#define PM_CHARSET_UCS_4_SWAPPED		"UCS-4-SWAPPED"
#define PM_CHARSET_C99					"C99"
#define PM_CHARSET_JAVA					"JAVA"
#define PM_CHARSET_ISO_8859_1			"ISO-8859-1"
#define PM_CHARSET_ISO_8859_2			"ISO-8859-2"
#define PM_CHARSET_ISO_8859_3			"ISO-8859-3"
#define PM_CHARSET_ISO_8859_4			"ISO-8859-4"
#define PM_CHARSET_ISO_8859_5			"ISO-8859-5"
#define PM_CHARSET_ISO_8859_6			"ISO-8859-6"
#define PM_CHARSET_ISO_8859_7			"ISO-8859-7"
#define PM_CHARSET_ISO_8859_8			"ISO-8859-8"
#define PM_CHARSET_ISO_8859_9			"ISO-8859-9"
#define PM_CHARSET_ISO_8859_10			"ISO-8859-10"
#define PM_CHARSET_ISO_8859_11			"ISO-8859-11"
#define PM_CHARSET_ISO_8859_13			"ISO-8859-13"
#define PM_CHARSET_ISO_8859_14			"ISO-8859-14"
#define PM_CHARSET_ISO_8859_15			"ISO-8859-15"
#define PM_CHARSET_ISO_8859_16			"ISO-8859-16"
#define PM_CHARSET_KOI8_R				"KOI8-R"
#define PM_CHARSET_KOI8_U				"KOI8-U"
#define PM_CHARSET_KOI8_RU				"KOI8-RU"
#define PM_CHARSET_CP1250				"CP1250"
#define PM_CHARSET_CP1251				"CP1251"
#define PM_CHARSET_CP1252				"CP1252"
#define PM_CHARSET_CP1253				"CP1253"
#define PM_CHARSET_CP1254				"CP1254"
#define PM_CHARSET_CP1255				"CP1255"
#define PM_CHARSET_CP1256				"CP1256"
#define PM_CHARSET_CP1257				"CP1257"
#define PM_CHARSET_CP1258				"CP1258"
#define PM_CHARSET_CP850				"CP850"
#define PM_CHARSET_CP862				"CP862"
#define PM_CHARSET_CP866				"CP866"
#define PM_CHARSET_MacRoman				"MacRoman"
#define PM_CHARSET_MacCentralEurope		"MacCentralEurope"
#define PM_CHARSET_MacIceland			"MacIceland"
#define PM_CHARSET_MacCroatian			"MacCroatian"
#define PM_CHARSET_MacRomania			"MacRomania"
#define PM_CHARSET_MacCyrillic			"MacCyrillic"
#define PM_CHARSET_MacUkraine			"MacUkraine"
#define PM_CHARSET_MacGreek				"MacGreek"
#define PM_CHARSET_MacTurkish			"MacTurkish"
#define PM_CHARSET_MacHebrew			"MacHebrew"
#define PM_CHARSET_MacArabic			"MacArabic"
#define PM_CHARSET_MacThai				"MacThai"
#define PM_CHARSET_HP_ROMAN8			"HP-ROMAN8"
#define PM_CHARSET_NEXTSTEP				"NEXTSTEP"
#define PM_CHARSET_ARMSCII_8			"ARMSCII-8"
#define PM_CHARSET_GEORGIAN_ACADEMY		"GEORGIAN-ACADEMY"
#define PM_CHARSET_GEORGIAN_PS			"GEORGIAN-PS"
#define PM_CHARSET_KOI8_T				"KOI8-T"
#define PM_CHARSET_PT154				"PT154"
#define PM_CHARSET_MULELAO_1			"MULELAO-1"
#define PM_CHARSET_CP1133				"CP1133"
#define PM_CHARSET_TIS_620				"TIS-620"
#define PM_CHARSET_CP874				"CP874"
#define PM_CHARSET_TCVN					"TCVN"
#define PM_CHARSET_JIS_C6220_1969_RO	"JIS_C6220-1969-RO"
#define PM_CHARSET_JIS_X0201			"JIS_X0201"
#define PM_CHARSET_JIS_X0208			"JIS_X0208"
#define PM_CHARSET_JIS_X0212			"JIS_X0212"
#define PM_CHARSET_GB_1988_80			"GB_1988-80"
#define PM_CHARSET_GB_2312_80			"GB_2312-80"
#define PM_CHARSET_ISO_IR_165			"ISO-IR-165"
#define PM_CHARSET_KSC_5601				"KSC_5601"
#define PM_CHARSET_EUC_JP				"EUC-JP"
#define PM_CHARSET_SHIFT_JIS			"SHIFT_JIS"
#define PM_CHARSET_CP932				"CP932"
#define PM_CHARSET_ISO_2022_JP			"ISO-2022-JP"
#define PM_CHARSET_ISO_2022_JP_1		"ISO-2022-JP-1"
#define PM_CHARSET_ISO_2022_JP_2		"ISO-2022-JP-2"
#define PM_CHARSET_GB2312				"GB2312"
#define PM_CHARSET_GBK					"GBK"
#define PM_CHARSET_CP936				"CP936"
#define PM_CHARSET_GB18030				"GB18030"
#define PM_CHARSET_ISO_2022_CN			"ISO-2022-CN"
#define PM_CHARSET_ISO_2022_CN_EXT		"ISO-2022-CN-EXT"
#define PM_CHARSET_HZ					"HZ"
#define PM_CHARSET_EUC_TW				"EUC-TW"
#define PM_CHARSET_BIG5					"BIG5"
#define PM_CHARSET_CP950				"CP950"
#define PM_CHARSET_BIG5_HKSCS_1999		"BIG5-HKSCS:1999"
#define PM_CHARSET_BIG5_HKSCS_2001		"BIG5-HKSCS:2001"
#define PM_CHARSET_BIG5_HKSCS			"BIG5-HKSCS"
#define PM_CHARSET_EUC_KR				"EUC-KR"
#define PM_CHARSET_CP949				"CP949"
#define PM_CHARSET_JOHAB				"JOHAB"
#define PM_CHARSET_ISO_2022_KR			"ISO-2022-KR"
#define PM_CHARSET_EUC_JISX0213			"EUC-JISX0213"
#define PM_CHARSET_SHIFT_JISX0213		"SHIFT_JISX0213"
#define PM_CHARSET_ISO_2022_JP_3		"ISO-2022-JP-3"
#define PM_CHARSET_BIG5_2003			"BIG5-2003"
#define PM_CHARSET_TDS565				"TDS565"
#define PM_CHARSET_ATARIST				"ATARIST"
#define PM_CHARSET_RISCOS_LATIN1		"RISCOS-LATIN1"
#define PM_CHARSET_DEC_KANJI			"DEC-KANJI"
#define PM_CHARSET_DEC_HANYU			"DEC-HANYU"
#define PM_CHARSET_CP437				"CP437"
#define PM_CHARSET_CP737				"CP737"
#define PM_CHARSET_CP775				"CP775"
#define PM_CHARSET_CP852				"CP852"
#define PM_CHARSET_CP853				"CP853"
#define PM_CHARSET_CP855				"CP855"
#define PM_CHARSET_CP857				"CP857"
#define PM_CHARSET_CP858				"CP858"
#define PM_CHARSET_CP860				"CP860"
#define PM_CHARSET_CP861				"CP861"
#define PM_CHARSET_CP863				"CP863"
#define PM_CHARSET_CP864				"CP864"
#define PM_CHARSET_CP865				"CP865"
#define PM_CHARSET_CP869				"CP869"
#define PM_CHARSET_CP1125				"CP1125"
#define PM_CHARSET_CP856				"CP856"
#define PM_CHARSET_CP922				"CP922"
#define PM_CHARSET_CP943				"CP943"
#define PM_CHARSET_CP1046				"CP1046"
#define PM_CHARSET_CP1124				"CP1124"
#define PM_CHARSET_CP1129				"CP1129"
#define PM_CHARSET_CP1161				"CP1161"
#define PM_CHARSET_CP1162				"CP1162"
#define PM_CHARSET_CP1163				"CP1163"

#define CHARSET_UNKNOWN( pszCharset ) (pszCharset == PM_NULL || pszCharset[0] == '\0') 
PM_BOOL bytestr_charset_match( ByteStr* bs1, ByteStr* bs2 );

int bytestr_regex_equal( ByteStr* bs, const char* pszPattern, const char* pszOptions );
int bytestr_regex_exist( ByteStr* bs, const char* pszPattern, const char* pszOptions );
PM_HANDLE bytestr_regex_search_begin( ByteStr* bs, const char* pszPattern, const char* pszOptions );
PM_BOOL bytestr_regex_search_next( PM_HANDLE hSearch, int* nStart, int* nLen );
void bytestr_regex_search_end( PM_HANDLE hSearch );
ByteStr* bytestr_regex_replace( ByteStr* bs, const char* pszPattern,const char* pszReplaceTo,const char* pszOptions );


PM_HANDLE bytestr_regex_match_begin( ByteStr* bs, const char* pszPattern, const char* pszOptions );
void bytestr_regex_match_end( PM_HANDLE hSearch );
ByteStr* bytestr_regex_match_get( PM_HANDLE hSearch, int i );
int bytestr_regex_match_get_ex( PM_HANDLE hSearch, int i, ByteStr* bsto );
int bytestr_regex_match_size( PM_HANDLE hSearch );

#ifdef __cplusplus
}
#endif

#endif

