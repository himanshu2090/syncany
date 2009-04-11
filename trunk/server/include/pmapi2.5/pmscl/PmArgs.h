#if !defined(__PmArgs__h__200699)
#define __PmArgs__h__200699

struct PM_ARGS_DATA;

/**	\brief	The PmArgs class

*/
class PmArgs
{
	PM_ARGS_DATA *m_data;

public:
	PmArgs();
	virtual ~PmArgs();

	PM_VOID load(PM_LPCSTR szArgs, PM_CHAR delimit);
	PM_VOID load(PM_LPCSTR szArgs);
	PM_LPCSTR operator[](PM_UINT nIndex);
	PM_VOID clear();
	PM_INT size();

	PM_VOID dump();
};
struct PmWordAnalyzerInfo;

#define PM_TOKEN_TYPE_SPECIAL	0x00000001
#define PM_TOKEN_TYPE_TOKEN		0x00000002
#define PM_TOKEN_TYPE_DIGIT		0x00000004
#define PM_TOKEN_TYPE_QSTR		0x00000008
#define PM_TOKEN_TYPE_COMMENT	0x00000010
#define PM_TOKEN_TYPE_STRING	0x00000020//分割出来的串


#define PM_TOKEN_TYPE_EOF		0x40000000
#define PM_TOKEN_TYPE_ERROR		0x80000000

#define PM_TOKEN_TYPE_NORMAL	PM_TOKEN_TYPE_TOKEN|PM_TOKEN_TYPE_DIGIT|PM_TOKEN_TYPE_QSTR|PM_TOKEN_TYPE_COMMENT
#define PM_TOKEN_TYPE_ALL		0xffffffff

#define PM_WAE_INVALID_CHAR		0x01
#define PM_WAE_CHAR_EXPECTED	0x02

struct PmWordAnalyzerInfo
{
	PM_LPCSTR pszText;
	PM_UINT nCurIndex;
	PM_UINT nSize;
	PM_DWORD dwAnaType;
	PM_LPCSTR pszSpecialTable;
	PM_UINT nSpecialTableLen;
	PM_LPCSTR pszDelimit;
	PM_UINT nDelimitLen;
	PM_BOOL bCurrent;
	PM_CHAR chCur;
	PM_UINT nCurLine;
	PM_DWORD dwErrCode;
};


class PmWordAnalyzer 
{
public:
	PmWordAnalyzer();
	virtual ~PmWordAnalyzer();
	PM_VOID loadData( PM_LPCSTR pszText, PM_UINT nSize, PM_LPCSTR pszDelimit = PM_NULL, PM_LPCSTR specialtable = PM_NULL, PM_DWORD anaType = PM_TOKEN_TYPE_NORMAL );
	PM_VOID loadString( PM_LPCSTR pszText, PM_LPCSTR pszDelimit = PM_NULL, PM_LPCSTR specialtable = PM_NULL, PM_DWORD anaType = PM_TOKEN_TYPE_NORMAL );
	PM_VOID loadString( const ByteStr* bs, PM_LPCSTR pszDelimit = PM_NULL, PM_LPCSTR specialtable = PM_NULL, PM_DWORD anaType = PM_TOKEN_TYPE_NORMAL );
	PM_VOID loadString( const PmString& str, PM_LPCSTR pszDelimit = PM_NULL, PM_LPCSTR specialtable = PM_NULL, PM_DWORD anaType = PM_TOKEN_TYPE_NORMAL );
	PM_DWORD nextToken( PmString& strToken );
	PM_BOOL readFileName(PmString& strFileName);
	PM_BOOL readSFloat(float & f);
	PM_DWORD getLineNumber();
	char getChar();
	PmWordAnalyzer& operator =( const PmWordAnalyzer& );
	PmWordAnalyzer( const PmWordAnalyzer&);
private:
	char getFirstChar();
	void moveBack1();
	PM_BOOL isSpecial( char ch );
	PM_BOOL isDelimit( char ch );

	PM_BOOL useSpecial();
	PM_BOOL useComment();
	PM_BOOL useString();
	PM_BOOL useQStr();
	PM_BOOL useDigit();
	PM_BOOL useToken();

	PM_DWORD nextTokenReal( PmString& str );

	PM_DWORD handleComment(PmString& str, char ch );
	PM_DWORD handleString(PmString& str, char ch );
	PM_DWORD handleQStr( PmString& str, char ch );
	PM_DWORD handleDigit( PmString& str, char ch );
	PM_DWORD handleToken( PmString& str, char ch );

	struct PmWordAnalyzerInfo* m_pInfo;
};


#endif
