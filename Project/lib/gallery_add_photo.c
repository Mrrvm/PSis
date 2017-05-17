#include "lib.h"

uint32_t gallery_add_photo(int peer_socket, char *file_name) {
	
	photo_data photo_data_;
	FILE *photo;
	int photo_size, size_buff;

	photo = fopen(file_name, "rb");
	fseek(photo, 0L, SEEK_END);
	photo_size = ftell(photo);
	fseek(photo, 0L, SEEK_SET);

	char * buffer=malloc(photo_size);
	size_buff = htonl(photo_size);

	send(peer_socket, &size_buff, sizeof(size_buff), 0);
	
	int k =fread(buffer, 1, photo_size, photo);
    send(peer_socket, buffer, photo_size, 0);
	// strcpy(photo_data_.file_name, file_name);
	// photo_data_.id_photo = 0;
	// send(peer_socket, &photo_data_, sizeof(photo_data), 0);
}