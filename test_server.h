
#ifndef _TEST_SERVER
#define _TEST_SERVER

#include <string.h>
#include <sys/socket.h>


// HEAD (2+2+8+4+4), BODY (32+128+2+40)
#define pkt_head_size 20
#define agent_info_packet 278 
#define big_buffer 5242880 

#define us unsigned

#define PKT_ADD_BIN(pkt, data, size) pkt = memcpy(pkt, data, size) + size
#define PKT_ADD_STRING(pkt, data, size) pkt = (void *)strncpy((char *)pkt, data, size) + size

#define PKT_ADD_INT16(pkt, data) *((us short *)pkt) = (us short)data; pkt += sizeof(us short)
#define PKT_ADD_INT32(pkt, data) *((us int *)pkt) =  (us int)data; pkt += sizeof(us int)
#define PKT_ADD_INT64(pkt, data) *((us long long *)pkt) = (us long long)data; pkt += sizeof(us long long)


typedef struct {

	int recv_addr_sz;
	int recv_socket, send_socket;
	char buffer[65535];
	char sendbuf[65535];

	struct sockaddr_in send_sockaddr;
	struct sockaddr_in recv_sockaddr;

}svr_conn;

typedef struct {
	
	int seq;
	char version[32];
	int binary_length;
	char *binary;

}__attribute__((packed)) req_update_agent_t;

typedef struct {
	
	short type;
	short version;
	us long long time;
	int total_length;
	int count;
	us char *data;
}__attribute__((packed)) packet_header_t;

#endif // _TEST_SERVER

