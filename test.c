#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main()
{


	int recv_addr_sz;
	int recv_socket;
	int send_socket;
	char buffer[65535];
	char *send_buf;

	struct sockaddr_in send_sockaddr;
	struct sockaddr_in recv_sockaddr;

	printf("[test_packet_sender] start !!\n");

	send_socket = socket(PF_INET, SOCK_STREAM, 0);
	if(-1 == send_socket) {
		return -1;
	}

	memset(&send_sockaddr, 0, sizeof(send_sockaddr));
	send_sockaddr.sin_family = AF_INET;
	send_sockaddr.sin_port = htons(8000);
	send_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(-1 == bind(send_socket, (struct sockaddr*)&send_sockaddr, sizeof(send_sockaddr))){
		return -1;
	}

	if(-1 == listen(send_socket, 5)){
		return -1;
	}

//	while(1)
//	{
		recv_addr_sz = sizeof(recv_sockaddr);
		recv_socket = accept(send_socket, (struct sockaddr*)&recv_sockaddr, &recv_addr_sz);

		if(-1 == recv_socket) {
//			break;
		}
		printf("[test_packet_sender] connect client!!\n");

		recv(recv_socket, buffer, 65535, 0);

	return 0;		
}
