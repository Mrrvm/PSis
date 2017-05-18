#include "peer.h"

void handle_replication(int gw_sock) {

	photo_data *photo_data_ = NULL;
	FILE *photo;
	int photo_size = 0;
    char *buffer;

    photo_data_ = malloc(sizeof(photo_data));

    recv(gw_sock, photo_data_, sizeof(*photo_data_), 0);
    photo_data_->type = ntohs(photo_data_->type);
    
    if(photo_data_->type == ADD_PHOTO) {

        printf("Received photo\n");
        recv(gw_sock, &photo_size, sizeof(photo_size), 0);
        photo_size = ntohl(photo_size);
        buffer = malloc(photo_size);
        recv(gw_sock, buffer, photo_size, 0);
        photo = fopen("nude_received.jpg", "wb");
        fwrite(buffer, 1, photo_size, photo);
        fclose(photo);
        free(buffer);
    }

    free(photo_data_);
}