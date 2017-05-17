#include "peer.h"

void *handle_client(void * arg) {

	int client_socket = *(int *) arg;
	long photo_size;
	photo_data photo_data_;
	FILE *photo;

	recv(client_socket, &photo_size, sizeof(photo_size), 0);
	photo_size = ntohl(photo_size);
	char * buffer=malloc(photo_size);
	recv(client_socket, buffer, photo_size, 0);


	photo = fopen("photos/nude_received.png", "wb");
	fwrite(buffer, 1, photo_size, photo);
	fclose(photo);

	// recv(client_socket, &photo_data_, sizeof(photo_data), 0);
	// printf("%s\n", photo_data_.file_name);
}
