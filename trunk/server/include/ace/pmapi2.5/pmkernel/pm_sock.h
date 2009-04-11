#ifndef __PM_SOCK_H__
#define __PM_SOCK_H__


/**	
 *@file pm_sock.h
 *@brief 
*/

/**	\brief HOST_NAME_LEN
*/
#define HOST_NAME_LEN     100
/**	\brief ERROR_LEN
*/
#define ERROR_LEN       200
/**	\brief MAX_TRY
*/
#define MAX_TRY           2
/**	\brief BUFSIZE
*/
#define BUFSIZE       1024

/* Error Msg */
/**	\brief SOCKERR_HOSTNOTFOUND
*/
#define SOCKERR_HOSTNOTFOUND  1
/**	\brief SOCKERR_SOCKETERROR
*/
#define SOCKERR_SOCKETERROR   2
/**	\brief SOCKERR_CONNECTERROR
*/
#define SOCKERR_CONNECTERROR  3
/**	\brief SOCKERR_SENDERROR
*/
#define SOCKERR_SENDERROR   4
/**	\brief SOCKERR_RECEIVEERROR
*/
#define SOCKERR_RECEIVEERROR  5
/**	\brief SOCKERR_TIMEOUT
*/
#define SOCKERR_TIMEOUT     6
/**	\brief SOCKERR_BUFFEROVERFLOW
*/
#define SOCKERR_BUFFEROVERFLOW  7

#ifdef WIN32
#pragma comment(lib,"Ws2_32.lib")
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/**	\brief	The pm_sock_close function

	\param	sockfd	a parameter of type PM_SOCKET

	\return	int

	
*/
	int pm_sock_close(PM_SOCKET sockfd);
/**	\brief	The pm_sock_init function


	\return	int

	
*/
	int pm_sock_init();
/**	\brief	The pm_socket_end function


	\return	void

	
*/
	void pm_socket_end();

/**	\brief	The pm_sock_read function

	\param	fd	a parameter of type PM_SOCKET
	\param	buf	a parameter of type void*
	\param	n	a parameter of type int

	\return	int

	
*/
	int pm_sock_read(PM_SOCKET fd, void* buf, int n);
/**	\brief	The pm_sock_write function

	\param	fd	a parameter of type PM_SOCKET
	\param	buf	a parameter of type const void*
	\param	n	a parameter of type int

	\return	int

	
*/
	int pm_sock_write(PM_SOCKET fd, const void* buf, int n);
/**	\brief	The pm_sock_puts function

	\param	sockfd	a parameter of type PM_SOCKET
	\param	s	a parameter of type const char*

	\return	int

	
*/
	int pm_sock_puts(PM_SOCKET sockfd, const char* s);
/**	\brief	The pm_sock_readline function

	\param	sockfd	a parameter of type PM_SOCKET
	\param	buf	a parameter of type char*
	\param	bufsize	a parameter of type int

	\return	int

	
*/
	int pm_sock_readline(PM_SOCKET sockfd, char* buf, int bufsize);  // read a line from socket;

/**	\brief pm_sock_error
*/
	int pm_sock_error(void);

/**	\brief	The pm_tcp_open function

	\param	host	a parameter of type const char*
	\param	port	a parameter of type PM_PORT

	\return	PM_SOCKET

	
*/
	PM_SOCKET pm_tcp_open(const char* host, PM_PORT port);
/**	\brief	The pm_tcp_server_socket function

	\param	port	a parameter of type PM_PORT

	\return	PM_SOCKET

	
*/
	PM_SOCKET pm_tcp_server_socket(PM_PORT port);

/**	\brief	The pm_udp_socket function


	\return	int

	
*/
	int pm_udp_socket();
/**	\brief	The pm_udp_server_socket function

	\param	port	a parameter of type PM_PORT

	\return	PM_SOCKET

	
*/
	PM_SOCKET pm_udp_server_socket(PM_PORT port);
/**	\brief	The pm_udp_send function

	\param	host	a parameter of type const char*
	\param	port	a parameter of type PM_PORT
	\param	data_buf	a parameter of type char*
	\param	data_size	a parameter of type int

	\return	int

	
*/
	int pm_udp_send(const char* host, PM_PORT port, char* data_buf,
				  int data_size);
/**	\brief	The pm_udp_recv function

	\param	servfd	a parameter of type PM_SOCKET
	\param	buf	a parameter of type char*
	\param	bufsize	a parameter of type PM_ULONG

	\return	int

	
*/
	int pm_udp_recv(PM_SOCKET servfd, char* buf, PM_ULONG bufsize);
/**	\brief	The pm_udp_recv_ex function

	\param	servfd	a parameter of type PM_SOCKET
	\param	buf	a parameter of type char*
	\param	bufsize	a parameter of type PM_ULONG
	\param	ip	a parameter of type PM_ULONG*
	\param	port	a parameter of type PM_PORT*

	\return	int

	
*/
	int pm_udp_recv_ex(PM_SOCKET servfd, char* buf, PM_ULONG bufsize, PM_ULONG* ip,PM_PORT* port);


	PM_IPADDR_V4 pm_gethostbyname( PM_LPCSTR lpszHost);
/**	\brief	The pm_inet_addr function

	\param	lpszHost	a parameter of type PM_LPCSTR

	\return	PM_IPADDR_V4

	
*/
	PM_IPADDR_V4 pm_inet_addr(PM_LPCSTR lpszHost);
/**	\brief	The pm_inet_ntoa function

	\param	ip	a parameter of type PM_IPADDR_V4
	\param	buf	a parameter of type char*

	\return	char*

	
*/
	char* pm_inet_ntoa( PM_IPADDR_V4 ip, char* buf );
#ifdef __cplusplus
}
#endif

#endif
