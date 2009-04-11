#ifndef __PM_STREAM_DEF_20070104_H__
#define __PM_STREAM_DEF_20070104_H__
//pmapi内部使用的头文件
PmOutputStreamPtr PmGetFileOutputStreamByHandle( PM_HANDLE hFile );
PmOutputStreamPtr PmGetFileOutputStreamByName( const char* pszFileName );
PmOutputStreamPtr PmGetStringOutputStream( ByteStr* bs );

PmInputStreamPtr PmGetFileInputStreamByName( const char* pszFileName );
PmInputStreamPtr PmGetFileInputStreamByHandle( PM_HANDLE hFile );
PmInputStreamPtr PmGetMemInputStream( void* pdata, int len );


PmInputStreamPtr PmGetStdinStream();
PmOutputStreamPtr PmGetStderrStream();
PmOutputStreamPtr PmGetStdoutStream();

PmUnknownPtr PmGetSocketStream( PmTcpSocket& sock );

#endif
