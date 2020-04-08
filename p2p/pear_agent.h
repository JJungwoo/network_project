#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_LEN 1024

typedef struct pear_conn_st{
	int target_sockfd;
	int target_len;
	struct sockaddr_in targetaddr;
	char buffer[BUF_LEN];
}pconn;


