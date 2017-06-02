#include "lib.h"

int main(int argc, char *argv[])
{
	char *host = NULL;
	in_port_t port = 0;
	int connect_ret = 1;
	uint32_t id_photo;
	uint32_t *id_photos[100] = {0};
	char *photo_name;
	char *photo_name_ptr;
	char *keyword;

	// Checks if user inserted the write input
	if(argc != 3) {
		printf("Invalid execution. Please use:\n./program [hostname] [port]\n");
		exit(1);
	}	

	host = argv[1];
	port = (in_port_t) atoi(argv[2]);
	
	// Hopefully connects to a peer
	connect_ret = gallery_connect(host, port);
	if(connect_ret == -1) {
		printf("The gateway cannot be accessed.\n");
		exit(2);
	}
	else if(connect_ret == 0) {
		printf("No peers available.\n");
		exit(3);
	}
	else {
		// Adds a new photo to the gallery
		id_photo = gallery_add_photo(connect_ret, "photos/my_first_nude.jpg");
		printf("Received id_photo: %d\n", id_photo);

		gallery_add_keyword(connect_ret, id_photo, "nude");
		gallery_add_keyword(connect_ret, id_photo, "hot");

		id_photo = gallery_add_photo(connect_ret, "photos/nude2.png");
		printf("Received id_photo: %d\n", id_photo);
		
		gallery_add_keyword(connect_ret, id_photo, "trump");
		gallery_add_keyword(connect_ret, id_photo, "hot");

		gallery_search_photo(connect_ret, "hot", id_photos);
		
		usleep(50000);
		if(1 == gallery_get_photo(connect_ret, id_photo, "recv_nude"));
			printf("Received photo\n");

		if(1 == gallery_get_photo_name(connect_ret, id_photo, &photo_name_ptr))
			printf("Received name %s\n", photo_name_ptr);

		if( 1 == gallery_delete_photo(connect_ret, id_photo))
			printf("Deleted id_photo: %d\n", id_photo);

	}
	return 0;
}