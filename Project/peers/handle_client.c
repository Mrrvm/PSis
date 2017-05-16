#include "peer.h"

void *handle_client(void * arg) {

	int client_socket = *(int *) arg;
	int photo_size;
	photo_data photo_data_;
	FILE *photo;

	recv(client_socket, &photo_size, sizeof(photo_size), 0);
	photo_size = ntohs(photo_size);
	char buffer[photo_size];
	recv(client_socket, buffer, photo_size, 0);


	photo = fopen("photos/nude_received.jpg", "w");
	fwrite(buffer, 1, sizeof(buffer)+1, photo);
	fclose(photo);

	// recv(client_socket, &photo_data_, sizeof(photo_data), 0);
	// printf("%s\n", photo_data_.file_name);
}
