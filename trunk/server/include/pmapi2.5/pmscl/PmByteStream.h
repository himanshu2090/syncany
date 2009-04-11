/**	
 *@file PmByteStream.h 
 *@brief 字节流操作函数接口
*/


#ifndef __PM_PmByteStream_H__
#define __PM_PmByteStream_H__

/**	\brief	The PmByteStream class

*/
class PmByteStream// : public PmPrintable
{
public:
	PmByteStream();
	~PmByteStream();//不要派生此类
	PmByteStream(const ByteStr *bs);
	PmByteStream( const char ch );
	PmByteStream(ByteStr *bsobj, PM_BOOL bAttach);
	PmByteStream(const PmByteStream& s);
	PmByteStream(const char* psz);
	PmByteStream(const void* pdata, PM_ULONG len);
	void setAt( PM_ULONG index,PM_BYTE );
	PM_BYTE getAt( PM_ULONG index ) const ;
	operator ByteStr*();
	operator const ByteStr*() const;
	operator PM_BYTE*();
	operator const PM_BYTE*()const ;
	operator const char*();
	operator const char*() const;
	
	void writeEnd();//写入结束符'\0'
	PmByteStream& wrap( ByteStr* s );
	void attach( ByteStr* s );
	ByteStr* detach();
	const char* c_str();
	const char* c_str() const;
	int release();
	PM_ULONG size_real() const;
	PM_ULONG length() const;
	PM_ULONG c_strlen();
	void resize( PM_UINT uiSize);
//	PmByteStream& BindByteStream(const ByteStr *bs);
	PmByteStream& operator=(const ByteStr *bs);
	PmByteStream& operator=(const PmByteStream& obj);
	PmByteStream& operator=(PM_LPCSTR psz);
	PmByteStream& operator=(const char);
	int compare( const ByteStr* bs ) const;
	int compare( PM_LPCSTR psz ) const;
	int compare( const PmByteStream& bs ) const;


//	PmPrintable& format( PM_LPCSTR pszFormat, ... );
	void format( PM_LPCSTR pszFormat, ... );
	void formatAppend( PM_LPCSTR pszFormat, ... );
	void vFormatAppend( PM_LPCSTR pszFormat, PM_ARGLIST args );
	void vFormat( const char* pszFormat, PM_ARGLIST args );

	void base64Decode();
	void base64Encode();
	void copy_data(const void *data, PM_ULONG len);

	PmByteStream& operator+=( const PmByteStream& obj);
	PmByteStream& operator+=( const ByteStr* bs_obj );
	PmByteStream& operator+=( const char* psz );
	PmByteStream& operator+=( const char ch );
	bool operator!() const;
	operator bool() const;
	void fetchToken( PM_INT& fetchData );//strtok begin
	PM_BOOL nextToken(PmByteStream &,const char *pszDelimit,PM_INT& fetchData );//strtok
	int find( const char* ) const;
	PmByteStream substr( int pos, PM_UINT len ) const;
	void insert( int pos, const char* psz );
	void insert( int pos, const char* psz, int len );
	void replace( const char* pszMatchText, const char* pszNewText );
	void trim();
	void remove( PM_ULONG pos, PM_ULONG len );
	/*!
	    \brief 得到字符串的字符集
		\return 如果没有设置字符集，为PM_NULL, 否则返回
				us-ascii, gb2312, big5, UTF8, ucs-2, gb18030, GBK,UTF-8,hz-gb-2312, ISO-2022-JP, iso-8859-1,
				的一种
	*/
	PM_LPCSTR charset() const;
/**	\brief	The convert function
	\param[out]	objStr	a parameter of type PmString,结果存放到此对象内
	\param[in]	pszCharsetTo	a parameter of type PM_LPCSTR,目标字符集
	\return	是否转换成功
	\note	把当前字符集转到目标字符集，如果当前字符集没有设置，先猜再转
			目标字符集应该是大字符集(UTF-8, UNICODE等)，否则转码会容易失败
*/
	PM_BOOL convert( PmByteStream& objStr, PM_LPCSTR pszCharsetTo = PM_CHARSET_UTF8 );
/**	\brief	The convert function
	\param[in]	pszCharsetTo	a parameter of type PM_LPCSTR,目标字符集
	\return	是否转换成功
	\note	把当前字符集转到目标字符集，如果当前字符集没有设置，先猜再转
			目标字符集应该是大字符集(UTF-8, UNICODE等)，否则转码会容易失败
			转变自身
*/
	PM_BOOL convert( PM_LPCSTR pszCharsetTo = PM_CHARSET_UTF8 );
/**	\brief	set charset of this string
	\param[in]	charset_name	charset name to set
	\return	没有，如果设置的字符集不支持将会抛出异常
	\note	如果要清除已经设置的字符集，请setCharset( PM_NULL )或者setCharset("");
			当设置某字符集，并且此字符集不支持的时候，将抛出异常
			
			目前支持的字符集有(设置字符集不区分大小写)：

				获取名称			可以设置的名称
				us-ascii			US-ASCII
				gb2312				GB2312
				big5				BIG5
				UTF8				UTF8,UTF-8
				ucs-2				UCS-2
				gb18030				GB18030
				GBK					GBK
				GBKUTF-8hz-gb-2312	hz-gb-2312
				ISO-2022-JP			ISO-2022-JP	
				iso-8859-1			ISO-8859-1
*/
	void setCharset( PM_LPCSTR charset_name );

	PM_BOOL hasCharset();
/**	\brief	get charset of this string
	\return	返回字符串字符集,如果没有字符集，返回空串""
*/
	const char* getCharset() const;

/**	\brief	guess charset
	\return	猜当前串的字符集,如果没有字符集，返回空串""
*/
	PM_LPCSTR guessCharset();

/**	\brief	decode MIME string which is for example in format of =?GB2312?B?abcdefg=?=
	\return
 *      PM_FALSE decoded fail, charset is not supported or is not a supported MIME string or encoding method is not supported
		other OK
*/
	PM_BOOL mimeDecode();
	void mimeEncode();
	void toUpper();
	void toLower();
	PmByteStream right( PM_INT num );
	PmByteStream left( PM_INT num );

	PmStringListPtr split( const char* pszDelimit );
	PmStringListPtr splitByString( const char* pszSplit );

	/**	\brief	通过正则表达式判断相等
		\para	pszOptions 参数，可以支持参数i，表示是否区分大小写, 默认区分
		\return 是否相等，如果正则表达式错误将抛异常
	*/
	PM_BOOL regEqual( const char* pszPattern, const char* pszOptions = PM_NULL );

	/**	\brief	判断正则表达式是否在字符串中存在
		\para	pszOptions 参数，可以支持参数
				i,表示是否区分大小写, 默认区分, 
				m,是否是多行匹配，默认不是
		\return 是否相等，如果正则表达式错误将抛异常
	*/
	PM_BOOL regExist( const char* pszPattern, const char* pszOptions = PM_NULL );

	PmStringListPtr regMatch( const char* pszPattern, const char* pszOptions = PM_NULL );

	/**	\brief	正则表达式替换
		\para	pszOptions 参数，可以支持参数
				i,表示是否区分大小写, 默认区分, 
				m,是否是多行匹配，默认不是
				g,是否替换多次，默认只替换一次
		\return 如果正则表达式错误将抛异常
		例如：

	pszText = "This is a test\n"
		"这是一个测试\n"
		"这还是一个测试\n"
		"This is a tEst too\n";
		"       end..";

	pszResult1 = "This is a test\n"
		"\n\n"
		"\n\n"
		"This is a tEst too\n";
		"       end..";

	pszResult2 = "this is a test\n"
		"这是一个测试\n"
		"这还是一个测试\n"
		"this is a test too\n";
		"       end..";

	str = pszText;
	str.regReplace("^这.*$", "\n", "gm" );
	CPPUNIT_ASSERT( str == pszResult1 );

	str = pszText;
	str.regReplace("^THIS(.*)TEST", "this\\1test", "gmi");
	CPPUNIT_ASSERT( str == pszResult2 );
	*/
	void regReplace( const char* pszPattern, const char* pszReplaceTo, const char* pszOptions = PM_NULL );
protected:
	ByteStr* m_byteStr;

private:
	PM_BOOL isByteStrNil() const;};
/** \brief PMBS_DECLARE_OPERATOR_ALL
*/
#define PMBS_DECLARE_OPERATOR_ALL( opr_type ) \
	PMBS_DECLARE_OPERATOR( == , opr_type ) \
	PMBS_DECLARE_OPERATOR( != , opr_type ) \
	PMBS_DECLARE_OPERATOR( > , opr_type ) \
	PMBS_DECLARE_OPERATOR( >= , opr_type ) \
	PMBS_DECLARE_OPERATOR( < , opr_type ) \
	PMBS_DECLARE_OPERATOR( <= , opr_type )
/** \brief PMBS_DECLARE_OPERATOR
*/
#define PMBS_DECLARE_OPERATOR( opr, opr_type ) \
	inline bool operator opr ( const PmByteStream& right, const opr_type left ){\
	return ( right.compare( left ) opr 0 );\
}
PMBS_DECLARE_OPERATOR_ALL( PmByteStream& )

#undef PMBS_DECLARE_OPERATOR
/** \brief PMBS_DECLARE_OPERATOR
*/
#define PMBS_DECLARE_OPERATOR( opr, opr_type ) \
	inline bool operator opr ( const PmByteStream& right, const opr_type left ){\
	return ( right.compare( left ) opr 0 );\
}\
	inline bool operator opr ( const opr_type right, const PmByteStream& left ){\
	return ( (-left.compare( right )) opr 0 );\
}
PMBS_DECLARE_OPERATOR_ALL( char* )
PMBS_DECLARE_OPERATOR_ALL( ByteStr* )


/**	\brief	The operator+ function

	\param	bs1	a parameter of type const PmByteStream&
	\param	bs2	a parameter of type const PmByteStream&

	\return	inline PmByteStream

	
*/
inline PmByteStream operator+( const PmByteStream& bs1, const PmByteStream& bs2 ){
	return PmByteStream(bs1) += bs2;
}
/**	\brief	The operator+ function

	\param	bs1	a parameter of type const PmByteStream&
	\param	psz	a parameter of type PM_LPCSTR

	\return	inline PmByteStream

	
*/
inline PmByteStream operator+( const PmByteStream& bs1, PM_LPCSTR psz ){
	return PmByteStream(bs1) += psz;
}
/**	\brief	The operator+ function

	\param	psz	a parameter of type PM_LPCSTR
	\param	bs1	a parameter of type const PmByteStream&

	\return	inline PmByteStream

	
*/
inline PmByteStream operator+( PM_LPCSTR psz, const PmByteStream& bs1 ){
	PmByteStream str = psz;
	str.setCharset( bs1.getCharset() );
	str += bs1;
	return str;
}
/**	\brief	The operator+ function

	\param	bs	a parameter of type const PmByteStream&
	\param	pbs	a parameter of type const ByteStr*

	\return	inline PmByteStream

	
*/
inline PmByteStream operator+( const PmByteStream& bs, const ByteStr* pbs ){
	return PmByteStream(bs) += pbs;
}
/**	\brief	The operator+ function

	\param	pbs	a parameter of type const ByteStr*
	\param	bs	a parameter of type const PmByteStream&

	\return	inline PmByteStream

	
*/
inline PmByteStream operator+( const ByteStr* pbs,const PmByteStream& bs ){
	PmByteStream str((ByteStr*)pbs);
	str.setCharset( bs.getCharset() );
	str += bs;
	return str;
}
/**	\brief	The operator+ function

	\param	ch	a parameter of type const char
	\param	bs	a parameter of type const PmByteStream&

	\return	inline PmByteStream

	
*/
inline PmByteStream operator+( const char ch,const PmByteStream& bs ){
	PmByteStream str;
	str = ch;
	str.setCharset( bs.getCharset() );
	str += bs;
	return str;
}
/**	\brief	The operator+ function

	\param	bs	a parameter of type const PmByteStream&
	\param	ch	a parameter of type const char

	\return	inline PmByteStream

	
*/
inline PmByteStream operator+( const PmByteStream& bs,const char ch ){
	return PmByteStream(bs) += ch;
}

/**	\brief	Redefinition of PmByteStream as PmString
*/
typedef PmByteStream PmString;
/**	\brief	Redefinition of PmByteStream as PmBinary
*/
typedef PmByteStream PmBinary;

#endif
