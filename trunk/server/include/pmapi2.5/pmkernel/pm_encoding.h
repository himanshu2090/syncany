#ifndef __PM_ENCODING_H__
#define __PM_ENCODING_H__

/**	
 *@file pm_encoding.h
 *@brief 编码操作的C接口函数
*/

#ifdef __cplusplus
extern "C"
{
#endif

/// pm_des_ec
void pm_des_ec(const void *i_blk, void *o_blk, void *key);
/// pm_des_dc
void pm_des_dc(const void *i_blk, void *o_blk, void *key);
/// pm_des_ky
void pm_des_ky(void *kval, void *key);

short pm_encrypt(const void *InBuf, long InLen, void *OutBuf, void *pKey);
void pm_decrypt(const void *InBuf, long InLen, void *OutBuf, void *pKey);
short pm_mac(const void *InBuf, long InLen, void *MacBuf, void *pKey);
short pm_demac(const void *InBuf, long InLen, void *MacBuf, void *pKey);


/**\brief	The pm_base64_encode function
 *\param[in]	pcData	character string which you want to encode
 *\param[in]	iLen	the length of str
 *\param[out]	pOutBuffer 编码后的输出字符集【缓冲区大小至少为(iLen + 3 - iLen % 3) * 4 / 3 + 1)】
 *\param[out]	piLenRet 编码后的输出长度,此参数可以为 0
 *\return 0:成功，非0表示失败，编码后会填写结束符'\\0'
 */
PM_INT pm_base64_encode( PM_LPCVOID pcData, PM_INT iLen, PM_LPVOID pOutBuffer,PM_INT* piLenRet);
/**\brief	The pm_base64_decode function
 *\param[in]	pcData	character string which you want to decode
 *\param[in]	iLen	the length of str
 *\param[out]   pOutBuffer 解码后的输出字符集【缓冲区大小应至少为(iLen + 1)】
 *\param[out]	piLenRet	the length of returned string 
 *\return 0:成功，非0表示失败
 */
PM_INT pm_base64_decode( PM_LPCVOID pcData, PM_INT iLen, PM_LPVOID pOutBuffer,PM_INT* piLenRet );

//void CRC32(void *pBuf, long nLen, void *crc);
PM_INT pm_crc64_encode(void *pBuf, char *ostr);
char HexToChar( unsigned char uCh );
void BinToAscii( char *istr, char *ostr, int len );
//////////////////////////////////////////////////////////////////////////
/**	\brief	The pm_md5_context struct

*/
struct pm_md5_context
{
    PM_UINT uiTotal[2];
    PM_UINT uiState[4];
    PM_BYTE byBuffer[64];
};
/**\brief The pm_md5_starts function
 \param[out]	pCtx	a parameter of type struct md5_context *	
 \note md5使用方法
 \code
	struct md5_context ctx;
    PM_BYTE md5sum[16];
    pm_md5_starts( &ctx );
    pm_md5_update( &ctx, pbyBuffer, len);
    pm_md5_finish( &ctx, md5sum );
 \endcode
 */
PM_VOID pm_md5_starts( struct pm_md5_context * pCtx );
/**\brief	The pm_md5_update function 
 *\param[in,out]	pCtx	a parameter of type struct md5_context *
 *\param[in]	pbyInput	a parameter of type PM_BYTE *
 *\param[in]	uiLength	a parameter of type PM_UINT	
 */
PM_VOID pm_md5_update( struct pm_md5_context * pCtx, PM_BYTE *pbyInput, PM_UINT uiLength );
/**\brief	The pm_md5_finish function
 *\param[in]	pCtx	a parameter of type struct pm_md5_context *	
 *\param[out]	byDigest	a parameter of type PM_BYTE	
 */
PM_VOID pm_md5_finish( struct pm_md5_context *pCtx, PM_BYTE byDigest[16] );

////////////////////////////////////////////////////////////////////////////////
//PM_INT pm_fprintf( PM_HANDLE hFile, PM_LPCSTR format ,...);
//PM_INT pm_fgetpos(PM_HANDLE hFile,fpos_t *pos );
//PM_INT pm_fsetpos( PM_HANDLE hFile, const fpos_t *pos );
//PM_INT pm_ferror(PM_HANDLE hFile);


#ifdef __cplusplus
}
#endif

#endif
