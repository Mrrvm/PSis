#include "gateway.h"

void *handle_replication(void * arg) {

	list *servers_list = (list *)arg;
	photo_data *photo_data_;

	photo_data_ = malloc(sizeof(photo_data));
	
	while(1) {

		// Receive photo data
		recv(client_socket, photo_data_, sizeof(*photo_data_), 0);
		photo_data_->type = ntohs(photo_data_->type);

		if(photo_data_->type == ADD_PHOTO) {
			int photo_size;
			photo_data photo_data_;
			FILE *photo;

			recv(client_socket, &photo_size, sizeof(photo_size), 0);
			photo_size = ntohl(photo_size);
			char *buffer = malloc(photo_size);
			recv(client_socket, buffer, photo_size, 0);

			photo = fopen("photos/nude_received.jpg", "wb");
			fwrite(buffer, 1, photo_size, photo);
			fclose(photo);
		}

		// Send to other peers in the list
	}
}