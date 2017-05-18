#include "peer.h"

void *handle_client(void * arg) {

	stream_sockets *ssockets = (stream_sockets *)arg;
	int client_socket = (*ssockets).client_sock;
	int gw_socket = (*ssockets).gw_sock;
	photo_data *photo_data_;

	printf("Handling client\n");

	photo_data_ = malloc(sizeof(photo_data));
	recv(client_socket, photo_data_, sizeof(*photo_data_), 0);

	photo_data_->type = ntohs(photo_data_->type);
	printf("%s\n", photo_data_->file_name);

	if(photo_data_->type == ADD_PHOTO) {
		int photo_size;
		photo_data photo_data_;

		recv(client_socket, &photo_size, sizeof(photo_size), 0);
		photo_size = ntohl(photo_size);
		char *buffer = malloc(photo_size);
		recv(client_socket, buffer, photo_size, 0);
	}

	// Send photo data and photo to gateway
	send(gw_socket, photo_data_, sizeof(*photo_data_), 0);
	send(gw_socket, &photo_size, sizeof(photo_size), 0);
	send(gw_socket, buffer, photo_size, 0);

	free(photo_data_);
}

// Creates a new thread for each client
// Resolves client requests
// Must have syncronization between clients in photos
void *handle_clients(void * arg) {

	stream_sockets *ssockets = (stream_sockets *)arg;
	int error;
	pthread_t thr_client;

	listen(sock_stream, 20);
	printf("In clientS thread\n");

	while(1) {
        
        printf("Waiting connection from client...\n");

        (*ssockets).client_sock = accept((*ssockets).client_sock, NULL, NULL);
        
        error = pthread_create(&thr_client, NULL, handle_client, (void *)ssockets);
		if(error != 0) {
			perror("Unable to create thread to handle clients.");
			exit(-1);
		}
	}
}