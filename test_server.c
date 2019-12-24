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

#include <fcntl.h>

#include "test_server.h"
#include "common.h"

int system_down = 0;
int agent_check = 0;

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
	int ret, end_cnt = 2;
	svr_conn *svrConn = NULL; 
	
	svrConn = (svr_conn *)data;
	printf("Hi I'm server_recv function !! \n");
	printf("[server_recv] Data check recv_addr_sz: %d\n", svrConn->recv_addr_sz);

	while(end_cnt){
		memset(svrConn->buffer, 0, sizeof(svrConn->buffer));
		ret = recv(svrConn->recv_socket, svrConn->buffer, 65535, 0);
		if(ret != 0) {
			agent_check = 1;
			printf("recv data\n");
			printHex("test", svrConn->buffer, agent_info_packet);
			end_cnt--;
			if(!end_cnt)	// agent_check after update_agent recv 
				system_down = 1;
		}
		else {
			printf("else \n");
		}
		printf("check ret:(%d), end_cnt(%d) \n", ret, end_cnt);
		sleep(1);
	}

	system_down = 1;

	pthread_exit(0);
}

int remove_packet(void **data)
{

	free(((packet_header_t *)*data)->data);
	free(*data);

	return 0;
}

int make_packet(us char *data, size_t data_size)
{
	us char *pkt_data, *pkt_tmp;
	size_t pkt_data_size;
	packet_header_t pheader;

	pkt_data_size = pkt_head_size;

	pkt_data = (us char *)malloc(pkt_data_size);
	pkt_tmp = pkt_data;

	// input ex data
	pheader.type = 2004;
	pheader.version = 1;
	pheader.time = 0.0;
	pheader.total_length = (int) data_size;
	pheader.count = 1;
	pheader.data = data;

	PKT_ADD_INT16(pkt_tmp, pheader.type);
	PKT_ADD_INT16(pkt_tmp, pheader.version);
	



	return 0;
}

int make_update_packet(void *data_ptr)
{
	int fd;
	unsigned char *pkt_data, *pkt_tmp;
	size_t pkt_data_size;

	req_update_agent_t ru_agent;
	ru_agent.seq = 01;
	strcpy(ru_agent.version, "1.0.0");
	
	fd = open("./mongomon", O_RDONLY);
	if(-1 == fd) {
		printf("[make_update_packet] open has failed. (%d) \n", errno);
		return -1;
	}

	lseek(fd, 0, SEEK_END);
	ru_agent.binary_length = lseek(fd, 0, SEEK_CUR);
	lseek(fd, 0, SEEK_SET);

	ru_agent.binary = (char *)malloc(sizeof(char) * ru_agent.binary_length);
	read(fd, ru_agent.binary, ru_agent.binary_length);

	printf("[make_update_packet] binary_length(%d) \n", ru_agent.binary_length);

	pkt_data_size = (4 + 32 + 4 + ru_agent.binary_length);
	
	pkt_data = (us char *)malloc(pkt_data_size);
	pkt_tmp = pkt_data;

	PKT_ADD_INT32(pkt_tmp, ru_agent.seq);
	PKT_ADD_STRING(pkt_tmp, ru_agent.version, sizeof(ru_agent.version));
	PKT_ADD_INT32(pkt_tmp, ru_agent.binary_length);
	PKT_ADD_BIN(pkt_tmp, ru_agent.binary, ru_agent.binary_length);

	make_packet(pkt_data, pkt_data_size);

	close(fd);

	return ru_agent.binary_length;
}

void *server_send(void *data)
{
	svr_conn *svrConn = NULL;
	svrConn = (svr_conn *)data;


	printf("start server_send() \n");

	while(1)
	{
		if(agent_check)
		{
			

			break;	
		}

		sleep(1);
	}

	pthread_exit(0);
}

void create_test_server(svr_conn *svrConn)
{
	int ret = 0;
	pthread_t thread_id;

	printf("[create_test_server] start !! \n");
	printf("[create_test_server] Data check recv_addr_sz: %d\n", svrConn->recv_addr_sz);

	ret = pthread_create(&thread_id, NULL, server_recv, (void *)svrConn);
	if(0 == ret) {
		printf("[create_test_server] success to create thread. name: server_recv, thread create: %lu \n", thread_id);
	} 
	else {
		printf("[create_test_server] failed to create thread. name: server_recv, (%d) \n", errno);
	}
				
	ret = pthread_create(&thread_id, NULL, server_send, (void *)svrConn);
	if(0 == ret) {
		printf("[create_test_server] success to create thread. name: server_send, thread create: %lu \n", thread_id);
	}
	else {
		printf("[create_test_server] failed to create thread. name: server_send, (%d) \n", errno);
	}
}

void uninit_system(svr_conn *svrConn)
{
	close(svrConn->recv_socket);
	close(svrConn->send_socket);
	free(svrConn);
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

	uninit_system(svrConn);

	printf("Test Server Exit \n");

	return 0;
}
