#ifndef __BYTESTR_CHARSET_H__
#define __BYTESTR_CHARSET_H__
/*
��Ҫֱ�Ӱ�����ʹ�ñ��ļ�����ĺ�����
��Ӧ��ʹ��PmString( PmByteStream )�����Ա����
2006 11 07 liubin
*/

#ifdef __cplusplus
extern "C"
{
#endif

/**	\brief	bytestr_set_charset ����ָ�����ַ���
	\param	bs	ԴByteStr
	\param	charset_name	��Ҫ���õ��ַ�������
	\return	0 on success, -1 if charset is not supported	
*/
PM_INT bytestr_set_charset(ByteStr* bs, PM_LPCSTR charset_name);

///�ɹ��򷵻�ByteStr���ַ������ƣ�ʧ�ܷ���PM_NULL 
PM_LPCSTR bytestr_get_charset(ByteStr *bs);

///�²�ByteStr�������ַ������ɹ������ַ��������ƣ�ʧ�ܷ���PM_NULL
PM_LPCSTR bytestr_guess_charset(ByteStr* bs);


///��һ���ַ���ת������һ���ַ�����return 0 on success��return -1 on error
PM_INT bytestr_convert_charset(ByteStr* bs, PM_LPCSTR from, PM_LPCSTR to);

/**
 *	decode MIME string which is for example in format of =?GB2312?B?abcdefg=?=
 *  Return value:
 *		0 string is decoded, charset is set
 *      1 string is decoded, but charset is not supported
 *      2 original string is not a supported MIME string
 *      3 encoding method is not supported
 *      -1 empty string is passed
 */
PM_INT bytestr_mimestring_decode(ByteStr* subject);

/**	\brief	The bytestr_mimestring_encode function 	format of =?GB2312?B?abcdefg=?
	\param	subject	a parameter of type ByteStr*
	\return	PM_INT		
*/	
PM_INT bytestr_mimestring_encode(ByteStr* subject);		//hhl----2006-9-13--add

const char* bytestr_get_charset_valid_name( const char* pszCharsetAlias );
PM_BOOL bytestr_charset_can_convert_to( const char* pszTo );
int pm_convert_real(const char *from, const char *to, const ByteStr* bsfrom, ByteStr* bsto, 
					double* pFailRate, PM_BOOL bTest  );//bTestΪ�棬������־

#ifdef __cplusplus
}
#endif
#endif

