#include "peer.h"

void *handle_client(void * arg) {

	int client_socket = *(int *) arg;
	int photo_size;
	photo_data photo_data_;
	FILE *photo;

	recv(client_socket, &photo_size, sizeof(photo_size), 0);
	photo_size = ntohl(photo_size);
	char * buffer=malloc(photo_size);
	recv(client_socket, buffer, photo_size, 0);

	photo = fopen("photos/nude_received.jpg", "wb");
	fwrite(buffer, 1, photo_size, photo);
	fclose(photo);

	// recv(client_socket, &photo_data_, sizeof(photo_data), 0);
	// printf("%s\n", photo_data_.file_name);
}

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