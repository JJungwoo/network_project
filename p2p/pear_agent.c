#include "pear_agent.h"

pconn gpconn;

void print_info()
{
	//printf("============================= \n");
	printf("############################# \n");
	printf("#    pear to pear agent     # \n");
	printf("############################# \n");
	//printf("============================= \n");
	printf("-h : Help to p2p agent usage \n");
	printf("-v : Confirm to p2p agent version \n");
}

int create_socket()
{
	int ret = 0;
	int opt_val = 1;

	ret = gpconn.recv_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(ret == FAIL) 
	{
		printf("[create_socket] socket failed (ret:%d) \n", ret);
		return FAIL;
	}

	setsockopt(gpconn.recv_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val));

	return ret;
}

int create_conn_st(int port)
{
	int ret = 0;

	gpconn.recvaddr.sin_family = AF_INET;
	gpconn.recvaddr.sin_port = htons(port);
	//gpconn.recvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	gpconn.recvaddr.sin_addr.s_addr = htonl(atoi("127.0.0.1"));

	ret = bind(gpconn.recv_sockfd, (struct sockaddr*)&gpconn.recvaddr, sizeof(gpconn.recvaddr));
	if(ret == FAIL)
	{
		printf("[create_conn_st] bind failed (ret:%d) %d \n", ret, errno);
		return FAIL;
	}

	ret = listen(gpconn.recv_sockfd, 5);
	if(ret == FAIL)
	{
		printf("[create_conn_st] listen failed (ret:%d) \n", ret);
		return FAIL;
	}

	return ret;
}

void close_socket()
{
	close(gpconn.recv_sockfd);
}

void signal_handler(int signo)
{
	printf("signal_hander \n");
	signal(SIGINT, SIG_DFL);
	close_socket();
}

int main(int argc, char **argv)
{
	int ret = 0;
	if (argc-1 <= 0)
	{
		printf("bad usage\n");
		print_info();
		return OK;
	}

	for(int i=0;i<argc;i++)
	{
		if(*argv[i] == '-')
		{
			switch(*(argv[i]+1))
			{
				case 'h':
				print_info();
				return OK;
				break;
			}
		}
		
	}

	signal(SIGINT, signal_handler);
	/*while(1){
		printf("TEST\n");
		sleep(10);
	}*/

	ret = create_socket();
	if(ret == FAIL) 
	{
		printf("create socker failed (ret:%d) \n", ret);
		return FAIL;
	}

	printf("%d \n", gpconn.recv_sockfd);

	ret = connect(gpconn.recv_sockfd, (struct sockaddr*)&gpconn.recvaddr, sizeof(gpconn.recvaddr));
	if(ret == FAIL)
	{

		printf("ret : %d \n", ret); 


		ret = create_conn_st(9999);
		if(ret == FAIL) 
		{
			printf("create_conn_st failed (ret:%d) \n", ret);
			return FAIL;
		}

		printf("No execute to target server \n");
		//printF("connect failed (ret:%d) \n", ret);
		//return FAIL;

		while(1)
		{
			printf("Listening...\n");
			gpconn.send_sockfd = accept(gpconn.recv_sockfd, 
				(struct sockaddr*)&gpconn.recvaddr, (socklen_t*)sizeof(gpconn.sendaddr));

			if(gpconn.send_sockfd == FAIL)
			{
				printf("accept failed (ret:%d) \n", gpconn.send_sockfd);
				return FAIL;
			}

			printf("Connected Client \n");
		}
	}
	else {
		printf("Connected Success \n");
	}

	close_socket();

	return 0;
}
