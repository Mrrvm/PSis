#include "peer.h"

void *handle_client(void * arg) {

	int client_socket = *(int *) arg;
	int photo_size;
	photo_data photo_data_;
	char *buffer;
	FILE *photo;

	recv(client_socket, &photo_size, sizeof(photo_size), 0);

	buffer = malloc(photo_size);
	recv(client_socket, buffer, photo_size, 0);

	photo = fopen("nude_received.jpg", "w");
	fwrite(buffer, 1, sizeof(buffer), photo);
	fclose(photo);

	recv(client_socket, photo_data_, sizeof(photo_data_), 0);
}