/*
 * Test Server 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <ctype.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <pthread.h>
#include <errno.h>

#include "test_server.h"
#include "common.h"

int system_down = 0;

svr_conn *createConn() 
{
	svr_conn *svrConn;

	svrConn = (svr_conn *)malloc(sizeof(svr_conn));
	memset(svrConn, 0, sizeof(svrConn));
	
	svrConn->send_socket = socket(PF_INET, SOCK_STREAM, 0);
	if(-1 == svrConn->send_socket) {
		printf("socket -1 \n");
		return NULL;
	}

	memset(&svrConn->send_sockaddr, 0, sizeof(svrConn->send_sockaddr));
	svrConn->send_sockaddr.sin_family = AF_INET;
	svrConn->send_sockaddr.sin_port = htons(8000);
	svrConn->send_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(-1 == bind(svrConn->send_socket, (struct sockaddr*)&svrConn->send_sockaddr, 
				sizeof(svrConn->send_sockaddr))) {
		printf("bind -1 \n");
		return NULL;
	}
	
	if(-1 == listen(svrConn->send_socket, 5)) {
		printf("listen -1 \n");
		return NULL;
	}

	svrConn->recv_addr_sz = sizeof(svrConn->recv_sockaddr);
	svrConn->recv_socket = accept(svrConn->send_socket, (struct sockaddr*)&svrConn->recv_sockaddr, 
			&svrConn->recv_addr_sz);

	if(-1 == svrConn->recv_socket) {
		printf("accpet -1 \n");
	}

	return svrConn;
}

void *server_recv(void *data) 
{
	int ret, end_cnt = 3;
	svr_conn *svrConn = NULL; 
	
	svrConn = (svr_conn *)data;
	printf("Hi I'm server_recv function !! \n");
	printf("[server_recv] Data check recv_addr_sz: %d\n", svrConn->recv_addr_sz);

	while(end_cnt){
		memset(svrConn->buffer, 0, sizeof(svrConn->buffer));
		ret = recv(svrConn->recv_socket, svrConn->buffer, 65535, 0);
		if(ret != 0) {
			printf("recv data\n");
			printHex("test", svrConn->buffer, agent_info_packet);
		}
		else {
			end_cnt--;
		}
		printf("check ret:(%d), end_cnt(%d) \n", ret, end_cnt);
		sleep(1);
	}

	system_down = 1;

	pthread_exit(0);
}

void server_send()
{

}

void create_test_server(svr_conn *svrConn)
{
	int ret = 0;
	pthread_t thread_id;

	printf("[create_test_server] start !! \n");
	printf("[create_test_server] Data check recv_addr_sz: %d\n", svrConn->recv_addr_sz);

	ret = pthread_create(&thread_id, NULL, server_recv, (void *)svrConn);
	if(ret == 0) {
		printf("[create_test_server] success to create thread. name: server_recv, thread create: %lu \n", thread_id);
	} 
	else {
		printf("[create_test_server] failed to create thread. name: server_recv, (%d) \n", errno);
	}
				

}

int main()
{
	svr_conn *svrConn = NULL;

	printf("Test Server Start \n");
	
	svrConn = createConn();
	printf("Data check recv_addr_sz: %d\n", svrConn->recv_addr_sz);

	create_test_server(svrConn);

	while(!system_down)
		sleep(1);

	free(svrConn);

	printf("Test Server Exit \n");

	return 0;
}
