#include "lib.h"

int main(int argc, char *argv[])
{
	char *host = NULL;
	in_port_t port = 0;
	int connect_ret = 1;
	int fcn_number = 0;
	uint32_t id_photo;
	char photo_name[30];
	int ret;

	uint32_t *id_photos[100] = {0};
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
		while(1) {
			printf("\n1 - ADD PHOTO\n2 - ADD KEYWORD\n3 - SEARCH PHOTO\n4 - DELETE PHOTO\n5 - GET PHOTO NAME\n6 - GET PHOTO\n");
			printf("[Insert the function number]: ");
			scanf("%d", &fcn_number);
			if(fcn_number == 1) {
				printf("[Insert file name]: ");
				scanf("%s", photo_name);
				// Adds a new photo to the gallery
				id_photo = gallery_add_photo(connect_ret, photo_name);
				printf("\nReceived id is: %d\n", id_photo);
			}
			else if(fcn_number == 2) {
				printf("[Insert keyword]: ");
				gallery_add_keyword(connect_ret, id_photo, "nude");
			}
			else if(fcn_number == 3) {
				
			}
			else if(fcn_number == 4) {
				
			}
			else if(fcn_number == 5) {
				
			}
			else if(fcn_number == 6) {
				printf("[Insert photo id and name of image to be downloaded]: ");
					scanf("%d %s", &id_photo, photo_name);
					//downloads foto
					ret = gallery_get_photo(connect_ret, id_photo, photo_name);
					if(ret == 1)
					{
						printf("\nPhoto downloaded successfully!\n");
					}
					else if(ret == 0){
						printf("\nInvalid id\n");
					}
					else if(ret == -1){
						printf("\nAn error ocurred\n");
					}

				
			}
			else {
				printf(KRED"Error"RESET": Invalid function number.\n");
				exit(-1);
			}
		}
		printf("Received id_photo: %d\n", id_photo);

		gallery_add_keyword(connect_ret, id_photo, "hot");

		id_photo = gallery_add_photo(connect_ret, "photos/nude2.png");
		printf("Received id_photo: %d\n", id_photo);
		
		gallery_add_keyword(connect_ret, id_photo, "trump");
		gallery_add_keyword(connect_ret, id_photo, "hot");

		gallery_search_photo(connect_ret, "hot", id_photos);
		
		
		if(1 == gallery_get_photo(connect_ret, id_photo, "recv_nude"));
			printf("Received photo\n");

		if(1 == gallery_get_photo_name(connect_ret, id_photo, &photo_name_ptr))
			printf("Received name %s\n", photo_name_ptr);

		if( 1 == gallery_delete_photo(connect_ret, id_photo))
		printf("Deleted id_photo: %d\n", id_photo);

	}
	return 0;
}