#ifndef __PM_FILEIO_H__
#define __PM_FILEIO_H__
/**	
 *@file pm_fileio.h
 *@brief �ļ�������C�ӿں���
 *@note ֻ֧��2G���´�С���ļ�
*/

#ifdef __cplusplus
extern "C"
{
#endif

///�ļ����ʵ�Ĭ�ϰ�ȫ���� 
#define	PM_FSA_NORMAL	0
///��ͼ����κ��û����ɷ���
#define	PM_FSA_LOW		1
///�ļ����ʵİ�ȫ���� ��PM_FSA_NORMALЧ��һ��
#define	PM_FSA_HIGH		2

///�ļ�ָ���ƶ���λ���,�ļ�ͷ��
#define PM_SEEK_SET	0
///�ļ�ָ���ƶ���λ���,��ǰλ��
#define PM_SEEK_CUR	1
///�ļ�ָ���ƶ���λ���,�ļ�β��
#define PM_SEEK_END	2

///�����ļ�ʹ�õĽṹ,pm_file_findfirst��pm_file_findnext����ʹ��
typedef struct _PM_FIND_DATA 
{  
	PM_BOOL bIsDirectory;				///<�Ƿ���Ŀ¼
	PM_TIME_T ftCreationTime;			///<����ʱ��
	PM_TIME_T ftLastAccessTime;			///<������ʱ��
	PM_TIME_T ftLastWriteTime;			///<������ʱ��
	PM_DWORD dwFileSize;				///<�ļ���С
	PM_CHAR cFileName[PM_MAX_PATH];		///<�ļ�·����
} PM_FIND_DATA, *PPM_FIND_DATA;

/**@brief	pm_file_create ����һ���ļ�(����һ�����ļ������һ���Ѿ����ڵ��ļ��������������)
 *@param[in]	pszcFileName �ļ����� 
 *@param[in]	bCreateAlways	�Ƿ����Ǵ��������bCreateAlwaysΪ�棬�����ļ�������񣬶�����һ���µ��ļ���ͬʱ����ԭ���ļ���
				���Ϊ��,���ļ��Ѿ�����ʱ��������ʧ�ܡ�
 *@param[in]	iFileSecureAttr	�ļ����ʰ�ȫ���𣬿�ȡPM_FSA_NORMAL��PM_FSA_LOW��PM_FSA_HIGH
 *@return	PM_HANDLE��ʧ�ܷ���PM_INVALID_HANDLE_VALUE�����򷵻��ļ����	
*/
PM_HANDLE pm_file_create( PM_LPCSTR pszcFileName,
						  PM_BOOL bCreateAlways,
						  PM_INT iFileSecureAttr );

/**	\brief	pm_file_open ���ļ�(��һ���Ѵ��ڵ��ļ����򴴽�һ�����ļ�)
	\param[in]	pszcFileName	�ļ���
	\param[in]	chAccessFlag	���ʱ��(ֻ����'r'��'R':��;'w'��'W':��д;'a'��'A':���ӣ������е�һ��)
	\param[in]	bOpenAlways	�Ƿ����Ǵ򿪣���bOpenAlwaysΪ�棬����ļ������ڣ�
	�򴴽�,����ļ����ڣ���򿪣������ļ�β������д(bAlwaysNew='a')
	*��bOpenOnlyΪ��,����ļ������ڣ����ʧ��
	\param[in]	iFileSecureAttr	�ļ����ʰ�ȫ���𣬿�ȡPM_FSA_NORMAL��PM_FSA_LOW��PM_FSA_HIGH
	\return	PM_HANDLE��ʧ�ܷ���PM_INVALID_HANDLE_VALUE�����򷵻��ļ���� 
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
	\return	PM_INT ������ֵ��PM_ERROR(-1)��ʾ��ȡʧ�ܣ����򷵻�ʵ�ʶ�ȡ���ֽ���
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

/**	\brief	The pm_file_eof �Ƿ����ļ�β��
	\param[in]	hFile	�ļ����
	\return	1:���ļ�β��0:��-1:����ʧ�� 
*/
PM_INT pm_file_eof(PM_HANDLE hFile);

/**	\brief	The pm_file_getchar ���ļ��ֶ�ȡһ���ַ�
	\param[in]	hFile	�ļ����
	\return	���ɹ��򷵻ض�ȡ���ַ���ʧ���򷵻�PM_ERROR
*/
PM_INT pm_file_getchar(PM_HANDLE hFile);


/**	\brief	The pm_file_putchar ���ļ���дһ���ַ�
	\param[in]	hFile	�ļ����
	\param[in]	chValue	Ҫд���ַ�
	\return	�ɹ�����1��ʧ�ܷ���PM_ERROR	
*/
PM_INT pm_file_putchar(PM_HANDLE hFile,const PM_CHAR chValue); 

/**	\brief	The pm_file_flush �ѻ������е�����ȫ��д�뵽�ļ� 
	\param[in]	hFile	�ļ����
	\return	ʧ�ܷ���0���ɹ����ط���
*/
PM_BOOL pm_file_flush(PM_HANDLE hFile);

/**	\brief	The pm_file_unlink ɾ���ļ�	
	\param[in]	pszcFileName	����ɾ�����ļ�����
	\return	ʧ�ܷ���0���ɹ����ط���	
*/
PM_BOOL pm_file_unlink(PM_LPCSTR pszcFileName);	  ///< =DeleteFile

/**	\brief	The pm_file_trunc �����ļ�ָ�뵱ǰλ��Ϊ�ļ�ĩβ
	\param[in]	hFile	�ļ����
	\return	ʧ�ܷ���0���ɹ����ط���
*/
PM_BOOL pm_file_trunc(PM_HANDLE hFile);

/**	\brief	The pm_file_gettime ��ȡ�ļ�����ʱ�䡢���һ�η���ʱ�䡢���һ���޸�ʱ��
	\param[in]	hFile	�ļ����
	\param[out]	pCreationTime	�ļ�����ʱ��
	\param[out]	pLastAccessTime	���һ�η���ʱ��
	\param[out]	pLastWriteTime	���һ���޸�ʱ��
	\return	ʧ�ܷ���0���ɹ����ط���	
*/
PM_BOOL pm_file_gettime(PM_HANDLE hFile,PM_TIME_T* pCreationTime, PM_TIME_T* pLastAccessTime,
			PM_TIME_T* pLastWriteTime);

/**	\brief	The pm_file_gettime_byname ��ȡ�ļ�����ʱ�䡢���һ�η���ʱ�䡢���һ���޸�ʱ��
	\param[in]	pszcFileName	�ļ�����
	\param[out]	pCreationTime	�ļ�����ʱ��
	\param[out]	pLastAccessTime	���һ�η���ʱ��
	\param[out]	pLastWriteTime	���һ���޸�ʱ��
	\return	ʧ�ܷ���0���ɹ����ط���	
*/
PM_BOOL pm_file_gettime_byname(PM_LPCSTR pszcFileName,PM_TIME_T* pCreationTime, PM_TIME_T* pLastAccessTime,
			PM_TIME_T* pLastWriteTime);

/**	\brief	The pm_file_settime �����ļ�����ʱ�䡢���һ�η���ʱ�䡢���һ���޸�ʱ��
	\note unix�²������ô���ʱ��,���������ĳ�������дPM_NULL)
	\param[in]	pszcFileName	�ļ�����
	\param[in]	pCreationTime	�ļ�����ʱ��(������PM_NULL)
	\param[in]	pLastAccessTime	���һ�η���ʱ��(������PM_NULL)
	\param[in]	pLastWriteTime	���һ���޸�ʱ��(������PM_NULL)
	\return	ʧ�ܷ���0���ɹ����ط���	
*/
PM_BOOL pm_file_settime(PM_LPCSTR pszcFileName,const PM_TIME_T* pCreationTime,const PM_TIME_T* pLastAccessTime,
			const PM_TIME_T* pLastWriteTime);

/**	\brief	The pm_file_getsize ��ȡ�ļ���С
	\param[in]	hFile �ļ���� 
	\return	�ɹ��򷵻��ļ�ʵ�ʴ�С��ʧ���򷵻�PM_ERROR	
*/
PM_INT pm_file_getsize(PM_HANDLE hFile);
PM_INT pm_file_getsize_by_name(const char* pszName);

/**	\brief	The pm_file_copy �ļ����� 
	\param[in]	lpExistingFileName	�Ѵ��ڵ��ļ�����
	\param[in]	lpNewFileName	Ҫ���Ƴɵ����ļ�����
	\param[in]	bFailIfExists	�����Ƿ�ʧ�ܣ���bFailIfExists=PM_TRUE,��������ƴ��Ѿ�������
	*���ļ�������ͬ���ļ�����ô�ͷ���ʧ�ܣ����򸲸Ǹ��ļ�
	\return	ʧ�ܷ���0���ɹ����ط���	
*/
PM_BOOL pm_file_copy(PM_LPCSTR lpExistingFileName, PM_LPCSTR lpNewFileName,PM_BOOL bFailIfExists);

/**	\brief	The pm_file_move �������ļ���Ŀ¼
	\param[in]	lpExistingFileName	Դ�ļ���Ŀ¼
	\param[in]	lpNewFileName	���ļ�����Ŀ¼��
	\return	ʧ�ܷ���0���ɹ����ط���	
*/
PM_BOOL pm_file_move(PM_LPCSTR lpExistingFileName,PM_LPCSTR lpNewFileName);


/**	\brief	pm_file_findfirst �����ļ���Ŀ¼
	\param[in]	pszcFileName	��Ҫ���ҵ��ļ�����
	\param[out]	pFileData �鵽��õ����ļ�����	
	\return	ʧ�ܷ���PM_INVALID_HANDLE_VALUE ,���򷵻�PM_HANGLE,��pm_file_findnext��pm_file_findcloseʹ�� 
	\note ���ɹ���Ҫ����pm_file_findclose�ͷ���Դ
*/
PM_HANDLE pm_file_findfirst(PM_LPCSTR pszcFileName,PM_FIND_DATA* pFileData);

/**	\brief	pm_file_findnext ������һ���ļ�
	\param[in]	hSearchHandle ��pm_file_findfirst���صľ��	
	\param[out]	pFileData �鵽��õ����ļ�����	
	\return	�ɹ�����PM_TRUE��ʧ�ܷ���PM_FALSE
*/
PM_BOOL pm_file_findnext(PM_HANDLE hSearchHandle, PM_FIND_DATA* pFileData);

/**	\brief	pm_file_findclose �رղ��Ҿ��
	\param	hSearchHandle	��pm_file_findfirst���صľ�� 
	\return	�ɹ�����PM_TRUE��ʧ�ܷ���PM_FALSE	
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
