#include "peer.h"

// Creates a new thread for each client
// Resolves client requests
// Must have syncronization between clients in photos
void *handle_clients(void * arg) {

	int sock_stream = *(int *) arg;
	int sock_client;
	socklen_t size_addr;
	conn_args *new_conn_args;
	int error;
	pthread_t thr_client;

	listen(sock_stream, 10);
	printf("In clientS thread\n");

	while(1) {
        
        printf("Waiting connection from client...\n");
        new_conn_args = malloc(sizeof(conn_args));
        size_addr = sizeof(new_conn_args->client_addr_);
        sock_client = accept(sock_stream, 
                    (struct sockaddr *) &new_conn_args->client_addr_, 
                    &size_addr);
        new_conn_args->sock_client_ = sock_client; 
        
        error = pthread_create(&thr_client, NULL, handle_client, (void *)new_conn_args);
		if(error != 0) {
			perror("Unable to create thread to handle clients.");
			exit(-1);
		}
	}
}