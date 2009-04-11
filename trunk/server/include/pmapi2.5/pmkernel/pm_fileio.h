#ifndef __PM_FILEIO_H__
#define __PM_FILEIO_H__
/**	
 *@file pm_fileio.h
 *@brief 文件操作的C接口函数
 *@note 只支持2G以下大小的文件
*/

#ifdef __cplusplus
extern "C"
{
#endif

///文件访问的默认安全级别 
#define	PM_FSA_NORMAL	0
///最低级别，任何用户都可访问
#define	PM_FSA_LOW		1
///文件访问的安全级别 和PM_FSA_NORMAL效果一样
#define	PM_FSA_HIGH		2

///文件指针移动定位标记,文件头部
#define PM_SEEK_SET	0
///文件指针移动定位标记,当前位置
#define PM_SEEK_CUR	1
///文件指针移动定位标记,文件尾部
#define PM_SEEK_END	2

///查找文件使用的结构,pm_file_findfirst和pm_file_findnext函数使用
typedef struct _PM_FIND_DATA 
{  
	PM_BOOL bIsDirectory;				///<是否是目录
	PM_TIME_T ftCreationTime;			///<创建时间
	PM_TIME_T ftLastAccessTime;			///<最后访问时间
	PM_TIME_T ftLastWriteTime;			///<最后更改时间
	PM_DWORD dwFileSize;				///<文件大小
	PM_CHAR cFileName[PM_MAX_PATH];		///<文件路径名
} PM_FIND_DATA, *PPM_FIND_DATA;

/**@brief	pm_file_create 创建一个文件(创建一个新文件，或打开一个已经存在的文件并清空所有内容)
 *@param[in]	pszcFileName 文件名称 
 *@param[in]	bCreateAlways	是否总是创建。如果bCreateAlways为真，不管文件存在与否，都创建一个新的文件，同时覆盖原有文件；
				如果为假,当文件已经存在时，创建会失败。
 *@param[in]	iFileSecureAttr	文件访问安全级别，可取PM_FSA_NORMAL、PM_FSA_LOW或PM_FSA_HIGH
 *@return	PM_HANDLE，失败返回PM_INVALID_HANDLE_VALUE，否则返回文件句柄	
*/
PM_HANDLE pm_file_create( PM_LPCSTR pszcFileName,
						  PM_BOOL bCreateAlways,
						  PM_INT iFileSecureAttr );

/**	\brief	pm_file_open 打开文件(打开一个已存在的文件，或创建一个新文件)
	\param[in]	pszcFileName	文件名
	\param[in]	chAccessFlag	访问标记(只能是'r'、'R':读;'w'、'W':读写;'a'、'A':附加，三种中的一种)
	\param[in]	bOpenAlways	是否总是打开，若bOpenAlways为真，如果文件不存在，
	则创建,如果文件存在，则打开，可在文件尾部附加写(bAlwaysNew='a')
	*若bOpenOnly为假,如果文件不存在，则打开失败
	\param[in]	iFileSecureAttr	文件访问安全级别，可取PM_FSA_NORMAL、PM_FSA_LOW或PM_FSA_HIGH
	\return	PM_HANDLE，失败返回PM_INVALID_HANDLE_VALUE，否则返回文件句柄 
*/
PM_HANDLE pm_file_open( PM_LPCSTR pszcFileName,
						PM_CHAR chAccessFlag,
						PM_BOOL bOpenAlways,
						PM_INT iFileSecureAttr	);

/**Close a file
 *@param[in] hFile file handle,returned by pm_file_open
 *@return 0:failure,not 0:success
 */
PM_BOOL pm_file_close( PM_HANDLE hFile );
 
/**	\brief	The pm_file_read function
	\param[in,out]	pBuffer	read buffer
	\param[in]	uiBytestoRead	number of bytes want to read
	\param[in]	hFile file handle,returned by pm_file_open
	\return	PM_INT 若返回值是PM_ERROR(-1)表示读取失败，否则返回实际读取的字节数
*/
PM_INT pm_file_read( PM_LPVOID pBuffer, PM_UINT uiBytestoRead,PM_HANDLE hFile );

/**	\brief	The pm_file_write function:write bytes to a file 
	\param[in]	pcBuffer buffer to write
	\param[in]	uiBytestoWrite	number of bytes want to write
	\param[in]	hFile file handle,returned by pm_file_open
	\return	PM_INT if fail return PM_ERROR, or return number of bytes written	
*/
PM_INT pm_file_write( PM_LPCVOID pcBuffer, PM_UINT uiBytestoWrite, PM_HANDLE hFile );

/**	\brief	The pm_file_seek function:set the file pointer
	\param[in]	hFile hFile file handle,returned by pm_file_open
	\param[in]	lOffset
	\param[in]	iOrigin	[PM_SEEK_SET,PM_SEEK_CUR,PM_SEEK_END]
	\return	PM_INT if fail return -1, or return the current position of the file pointer	
*/
PM_INT pm_file_seek( PM_HANDLE hFile, PM_LONG lOffset, PM_UINT iOrigin );

///\return	PM_INT if fail return PM_ERROR, or return the current position of the file pointer
PM_INT pm_file_tell( PM_HANDLE hFile );

/**	\brief	The pm_file_eof 是否是文件尾部
	\param[in]	hFile	文件句柄
	\return	1:是文件尾；0:否；-1:操作失败 
*/
PM_INT pm_file_eof(PM_HANDLE hFile);

/**	\brief	The pm_file_getchar 从文件种读取一个字符
	\param[in]	hFile	文件句柄
	\return	若成功则返回读取的字符，失败则返回PM_ERROR
*/
PM_INT pm_file_getchar(PM_HANDLE hFile);


/**	\brief	The pm_file_putchar 向文件种写一个字符
	\param[in]	hFile	文件句柄
	\param[in]	chValue	要写的字符
	\return	成功返回1，失败返回PM_ERROR	
*/
PM_INT pm_file_putchar(PM_HANDLE hFile,const PM_CHAR chValue); 

/**	\brief	The pm_file_flush 把缓冲区中的内容全部写入到文件 
	\param[in]	hFile	文件句柄
	\return	失败返回0，成功返回非零
*/
PM_BOOL pm_file_flush(PM_HANDLE hFile);

/**	\brief	The pm_file_unlink 删除文件	
	\param[in]	pszcFileName	所有删除的文件名称
	\return	失败返回0，成功返回非零	
*/
PM_BOOL pm_file_unlink(PM_LPCSTR pszcFileName);	  ///< =DeleteFile

/**	\brief	The pm_file_trunc 设置文件指针当前位置为文件末尾
	\param[in]	hFile	文件句柄
	\return	失败返回0，成功返回非零
*/
PM_BOOL pm_file_trunc(PM_HANDLE hFile);

/**	\brief	The pm_file_gettime 获取文件创建时间、最后一次访问时间、最后一次修改时间
	\param[in]	hFile	文件句柄
	\param[out]	pCreationTime	文件创建时间
	\param[out]	pLastAccessTime	最后一次访问时间
	\param[out]	pLastWriteTime	最后一次修改时间
	\return	失败返回0，成功返回非零	
*/
PM_BOOL pm_file_gettime(PM_HANDLE hFile,PM_TIME_T* pCreationTime, PM_TIME_T* pLastAccessTime,
			PM_TIME_T* pLastWriteTime);

/**	\brief	The pm_file_gettime_byname 获取文件创建时间、最后一次访问时间、最后一次修改时间
	\param[in]	pszcFileName	文件名称
	\param[out]	pCreationTime	文件创建时间
	\param[out]	pLastAccessTime	最后一次访问时间
	\param[out]	pLastWriteTime	最后一次修改时间
	\return	失败返回0，成功返回非零	
*/
PM_BOOL pm_file_gettime_byname(PM_LPCSTR pszcFileName,PM_TIME_T* pCreationTime, PM_TIME_T* pLastAccessTime,
			PM_TIME_T* pLastWriteTime);

/**	\brief	The pm_file_settime 设置文件创建时间、最后一次访问时间、最后一次修改时间
	\note unix下不能设置创建时间,如果不设置某项，该项填写PM_NULL)
	\param[in]	pszcFileName	文件名称
	\param[in]	pCreationTime	文件创建时间(可以是PM_NULL)
	\param[in]	pLastAccessTime	最后一次访问时间(可以是PM_NULL)
	\param[in]	pLastWriteTime	最后一次修改时间(可以是PM_NULL)
	\return	失败返回0，成功返回非零	
*/
PM_BOOL pm_file_settime(PM_LPCSTR pszcFileName,const PM_TIME_T* pCreationTime,const PM_TIME_T* pLastAccessTime,
			const PM_TIME_T* pLastWriteTime);

/**	\brief	The pm_file_getsize 获取文件大小
	\param[in]	hFile 文件句柄 
	\return	成功则返回文件实际大小，失败则返回PM_ERROR	
*/
PM_INT pm_file_getsize(PM_HANDLE hFile);
PM_INT pm_file_getsize_by_name(const char* pszName);

/**	\brief	The pm_file_copy 文件复制 
	\param[in]	lpExistingFileName	已存在的文件名称
	\param[in]	lpNewFileName	要复制成的新文件名称
	\param[in]	bFailIfExists	存在是否失败，若bFailIfExists=PM_TRUE,如果被复制处已经存在与
	*该文件名称相同的文件，那么就返回失败，否则覆盖该文件
	\return	失败返回0，成功返回非零	
*/
PM_BOOL pm_file_copy(PM_LPCSTR lpExistingFileName, PM_LPCSTR lpNewFileName,PM_BOOL bFailIfExists);

/**	\brief	The pm_file_move 重命名文件或目录
	\param[in]	lpExistingFileName	源文件或目录
	\param[in]	lpNewFileName	新文件名或目录名
	\return	失败返回0，成功返回非零	
*/
PM_BOOL pm_file_move(PM_LPCSTR lpExistingFileName,PM_LPCSTR lpNewFileName);


/**	\brief	pm_file_findfirst 查找文件或目录
	\param[in]	pszcFileName	所要查找的文件名称
	\param[out]	pFileData 查到后得到的文件属性	
	\return	失败返回PM_INVALID_HANDLE_VALUE ,否则返回PM_HANGLE,被pm_file_findnext、pm_file_findclose使用 
	\note 若成功后，要调用pm_file_findclose释放资源
*/
PM_HANDLE pm_file_findfirst(PM_LPCSTR pszcFileName,PM_FIND_DATA* pFileData);

/**	\brief	pm_file_findnext 查找下一个文件
	\param[in]	hSearchHandle 由pm_file_findfirst返回的句柄	
	\param[out]	pFileData 查到后得到的文件属性	
	\return	成功返回PM_TRUE，失败返回PM_FALSE
*/
PM_BOOL pm_file_findnext(PM_HANDLE hSearchHandle, PM_FIND_DATA* pFileData);

/**	\brief	pm_file_findclose 关闭查找句柄
	\param	hSearchHandle	由pm_file_findfirst返回的句柄 
	\return	成功返回PM_TRUE，失败返回PM_FALSE	
*/
PM_BOOL pm_file_findclose(PM_HANDLE hSearchHandle);
/** \brief pm_mkdir

*/
PM_INT pm_mkdir( PM_LPCSTR pszDir,PM_INT iFileSecureAttr );
/** \brief pm_mkdir_r

*/
PM_INT pm_mkdir_r( PM_LPCSTR pszDir,PM_INT iFileSecureAttr );
/** \brief pm_file_exist

*/
PM_BOOL pm_file_exist(PM_LPCSTR pszcFileName);
/** \brief pm_file_get_ext

*/
PM_LPCSTR pm_file_get_ext(PM_LPCSTR filename);
/** \brief pm_dir_exist

*/
PM_BOOL pm_dir_exist(PM_LPCSTR pszcDirName);


int pm_deltree( const char* pszDirName );

char* pm_file_readline( PM_HANDLE hFile, char* line, int len );
int pm_file_printf( PM_HANDLE hFile, const char * pszFormat, ...);
#ifdef __cplusplus
}
#endif

#endif
