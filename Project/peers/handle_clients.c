#include "peer.h"

void *handle_client(void * arg) {

	stream_sockets *ssockets = (stream_sockets *)arg;
	int client_socket = (*ssockets).client_sock;
	int gw_socket = (*ssockets).gw_sock;
	photo_data photo_data_;
	int photo_size = 0;
	char *buffer;
	int size_buff = 0;
	int res;

	printf("Handling client\n");

	while(1) {

		res = recv(client_socket, &photo_data_, sizeof(photo_data_), 0);
		if (res<=0){
			break;
		}
		else {
			if(sizeof(photo_data_) >= res && res > 0) {
				res = send(gw_socket, &photo_data_, sizeof(photo_data_), 0);
				if(ntohl(photo_data_.type) == ADD_PHOTO) {
					res = recv(client_socket, &photo_size, sizeof(photo_size), 0);
					res = send(gw_socket, &photo_size, sizeof(photo_size), 0);
					size_buff = ntohl(photo_size);
					buffer = malloc(size_buff);
					if(buffer == NULL){
						printf("buffer NULL;\n");
						exit(0);
					}
					recv(client_socket, buffer, size_buff, 0);
					send(gw_socket, buffer, size_buff, 0);
					free(buffer); 
				}
			}
			else {
				break;
			}
		}
	}
	//pthread_exit(arg);
	close(client_socket);
}

// Creates a new thread for each client
// Resolves client requests
// Must have syncronization between clients in photos
void *handle_clients(void * arg) {

	stream_sockets *ssockets = (stream_sockets *)arg;
	int error;
	pthread_t thr_client;
	int sock_stream = (*ssockets).client_sock;
	int ret;

	listen(sock_stream, 20);
	printf("In clients thread\n");

	while(1) {
        
        printf("Waiting connection from client...\n");

        (*ssockets).client_sock = accept(sock_stream, NULL, NULL);
        
        error = pthread_create(&thr_client, NULL, handle_client, (void *)ssockets);

        pthread_detach(thr_client);
		if(error != 0) {
			perror("Unable to create thread to handle clients.");
			exit(-1);
		}
	}
}