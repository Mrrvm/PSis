#include "lib.h"

uint32_t gallery_add_photo(int peer_socket, char *file_name) {
	
	// int test = htons(14);
	// printf("Adding %s\n", file_name);
	// send(peer_socket, &test, sizeof(test), 0);
	
	photo_data photo_data_;
	FILE *photo;
	long photo_size;

	photo = fopen(file_name, "rb");
	fseek(photo, 0, SEEK_END);
	photo_size = ftell(photo);
	fseek(photo, 0, SEEK_SET);
	char * buffer=malloc(photo_size);
	
	photo_size = htonl(photo_size);
	send(peer_socket, &photo_size, sizeof(photo_size), 0);
	
	fread(buffer, 1, photo_size, photo);
    send(peer_socket, buffer, photo_size, 0);
	
	// strcpy(photo_data_.file_name, file_name);
	// photo_data_.id_photo = 0;
	// send(peer_socket, &photo_data_, sizeof(photo_data), 0);
}