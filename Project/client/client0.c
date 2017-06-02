#include "lib.h"

int main(int argc, char *argv[])
{
	char *host = NULL;
	in_port_t port = 0;
	int connect_ret = 1;
	int fcn_number = 0;
	int ret = 0;
	int i = 0;
	uint32_t id_photo;
	uint32_t *id_photos[100] = {0};
	char photo_name[30];
	char keyword[30];
	char *photo_name_ptr;

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
				ret = gallery_add_photo(connect_ret, photo_name);
				if(ret == 0){
					printf("\nAn error ocurred\n");
				}
				else{
					printf("\nAdded photo with id %d\n", ret);
				}
			}
			else if(fcn_number == 2) {
				printf("[Insert photo id and keyword]: ");
				scanf("%d %s", &id_photo, keyword);
				ret = gallery_add_keyword(connect_ret, id_photo, keyword);
				if(ret == 1){
					printf("\nKeyword added successfully\n");
				}
				else if(ret == 0){
					printf("\nNo photo found with this id\n");
				}
				else if(ret == -1){
					printf("\nAn error ocurred\n");
				}

			}
			else if(fcn_number == 3) {
				printf("[Insert keyword]: ");
				scanf("%s", keyword);
				ret = gallery_search_photo(connect_ret, keyword, id_photos);
				if(ret == -1){
					printf("\nAn error ocurred\n");
				}
				else if(ret == 0){
					printf("\nFound 0 photos with this keyword\n");
				}
				else{
					printf("\nFound %d photos with this keyword\n", ret);
					printf("Found ids: ");
					while(id_photos[i] != 0) {
						printf("%d ", id_photos[i]);
						i++;
					}
					printf("\n");
				}
			}
			else if(fcn_number == 4) {
				printf("[Insert photo id]: ");
				scanf("%u", &id_photo);
				ret = gallery_delete_photo(connect_ret, id_photo);
				if(ret == -1){
					printf("\nAn error ocurred\n");
				}
				else if(ret == 0){
					printf("\nNo photos found with this id\n");
				}
				else if(ret == 1){
					printf("\nPhoto removed successfully\n");
				}
			}
			else if(fcn_number == 5) {
				
				printf("[Insert photo id]: ");
				scanf("%u", &id_photo);
				ret = gallery_get_photo_name(connect_ret, id_photo, &photo_name_ptr);
				if(ret == -1){
					printf("\nAn error ocurred\n");
				}
				else if(ret == 0){
					printf("\nNo photo found with this id\n");
				}
				else if(ret == 1){
					printf("\nReceived photo name: %s\n", photo_name_ptr);	
				}
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
						printf("\nNo photos found with this id\n");
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

	}
	return 0;
}