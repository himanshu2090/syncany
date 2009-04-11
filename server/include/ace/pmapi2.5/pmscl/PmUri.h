#ifndef __PM_URI_H__
#define __PM_URI_H__

/**	
 *@file PmUri.h 
 *@brief 
*/

/**	\brief	PM_IURI_NAME

*/
#define PM_IURI_NAME "IPmUri"
/**	\brief	The PmUri class

*/

class PmUri
{
public:
	PmUri();
	PM_BOOL parse( const char* pszUri);
	const char *getProtocol() const;
	const char *getUser() const;
	const char *getPass() const;
	const char *getHost() const;
	const char *getQHolder() const;
	const char *getQName() const;
	PM_UINT getPort() const;
	const char *buildUri(ByteStr*) const;
	void dump() const;
private:
	PM_CHAR		protocol[20];
	PM_CHAR		username[128];
	PM_CHAR		password[128];
	PM_CHAR		hostname[128];
	PM_CHAR		queueholder[128];
	PM_CHAR		queuename[128];
	PM_USHORT	port;
};

#endif
