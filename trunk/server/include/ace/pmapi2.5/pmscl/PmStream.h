#ifndef __PM_ISTREAM_H__
#define __PM_ISTREAM_H__
//公开接口
typedef enum tagStreamType
{
	PST_I_STDIN			= 0,
	PST_O_STDOUT		= 1,
	PST_O_STDERR		= 2, 
	PST_O_FILE_HANDLE	= 3,
	PST_I_FILE_HANDLE	= 4,
	PST_O_FILE_NAME		= 5,
	PST_I_FILE_NAME		= 6,
	PST_O_BYTESTR		= 7,
	PST_I_BYTESTR		= 8,
	PST_I_MEMORY		= 9,
	PST_I_SOCKET		= 10,
	PST_O_SOCKET		= 11,
}PM_STREAM_TYPE;

typedef struct tagPM_STREAM_BUF
{
	void* data;
	int len;
}PM_STREAM_BUF;

PmOutputStreamPtr PmGetOutputStream( PM_STREAM_TYPE stype = PST_O_STDOUT, PM_HANDLE hStreamData = PM_NULL );
PmInputStreamPtr PmGetInputStream( PM_STREAM_TYPE stype = PST_I_STDIN, PM_HANDLE hStreamData  = PM_NULL );

#define pmout (PmGetOutputStream())
#define pmerr (PmGetOutputStream(PST_O_STDERR))
#define pmin (PmGetInputStream())


void PmStreamPrintLine( PmOutputStreamPtr& pStream, char* psz );
void PmStreamPrint( PmOutputStreamPtr& pStream, char* psz );
void PmStreamPrintFmt( PmOutputStreamPtr& pStream, char* pszFomat, ... );
void PmStreamWrite( PmOutputStreamPtr& pStream, PM_BYTE* psz, int len );
void PmStreamPrintLineFmt( PmOutputStreamPtr& pStream, char* pszFormat, ... );

int PmStreamReadLine( PmInputStreamPtr& ptrStream, PmString& strLine );

class PmAbstractPrintStream : public IPmPrintStream, public IPmOutputStream
{
public:
	virtual void print( PM_LPCSTR pszFormat, ... );
	virtual void println( PM_LPCSTR pszFormat, ... );
	void vPrint( PM_LPCSTR pszFormat, PM_ARGLIST args );
};


#endif
