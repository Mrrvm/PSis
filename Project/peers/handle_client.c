#include "peer.h"

void *handle_client(void * arg) {

	int test;
	conn_args *new_conn_args = (conn_args *) arg;

	printf("ClienT thread created!\n");
	recv(new_conn_args->sock_client_, &test, sizeof(test), 0);	
	printf("Received: %d\n", ntohs(test));
	free(arg);
}