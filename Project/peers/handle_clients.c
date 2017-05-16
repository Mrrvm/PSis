#include "peer.h"

// Creates a new thread for each client
// Resolves client requests
// Must have syncronization between clients in photos
void *handle_clients(void * arg) {

	int sock_stream = *(int *) arg;
	int sock_client;
	int error;
	pthread_t thr_client;

	listen(sock_stream, 10);
	printf("In clientS thread\n");

	while(1) {
        
        printf("Waiting connection from client...\n");

        sock_client = accept(sock_stream, NULL, NULL);

        
        error = pthread_create(&thr_client, NULL, handle_client, &sock_client);
		if(error != 0) {
			perror("Unable to create thread to handle clients.");
			exit(-1);
		}
	}
}