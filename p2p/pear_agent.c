#include "pear_agent.h"

pconn gpconn;

void print_info()
{
	printf("=============================\n");
	printf("= pear to pear agent \n");
	printf("=============================\n");
}

int create_socket()
{
	if((gpconn.target_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
	{
		printf("socket failed \n");
		return -1;	
	}
	return 0;
}


int main(int argc, char *argv[])
{
	
	if(argc <= 1){
		printf("bad usage\n");
		return 0;
	}
	print_info();

	create_socket();
	printf("%d\n", gpconn.target_sockfd);


	return 0;
}
