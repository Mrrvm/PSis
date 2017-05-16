#include "lib.h"

uint32_t gallery_add_photo(int peer_socket, char *file_name) {
	
	// int test = htons(14);
	// printf("Adding %s\n", file_name);
	// send(peer_socket, &test, sizeof(test), 0);
	photo_data photo_data_;


	send(peer_socket, &photo_data_, sizeof(photo_data_));
}