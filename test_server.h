
#ifndef _TEST_SERVER
#define _TEST_SERVER

#include <sys/socket.h>

// HEAD (2+2+8+4+4), BODY (32+128+2+40)
#define agent_info_packet 278 

typedef struct {

	int recv_addr_sz;
	int recv_socket, send_socket;
	char buffer[65535];
	char sendbuf[65535];

	struct sockaddr_in send_sockaddr;
	struct sockaddr_in recv_sockaddr;

}svr_conn;


#endif // _TEST_SERVER

