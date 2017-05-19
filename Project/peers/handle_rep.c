#include "peer.h"

void handle_rep(int socket) {

    photo_data *photo_data_ = NULL;
    int photo_size = 0;
    char *buffer;
    FILE *photo;

    photo_data_ = malloc(sizeof(photo_data));

    recv(socket, photo_data_, sizeof(*photo_data_), 0);
    photo_data_->type = ntohs(photo_data_->type);
    
    if(photo_data_->type == ADD_PHOTO) {
        recv(socket, &photo_size, sizeof(photo_size), 0);
        photo_size = ntohl(photo_size);
        buffer = malloc(photo_size);
        recv(socket, buffer, photo_size, 0);
    	
    	printf("Received photo of size %d!\n", photo_size);
        
        photo = fopen("photos/nude_received.jpg", "wb");
        fwrite(buffer, 1, photo_size, photo);
        fclose(photo);
    }
	    free(buffer);
    
    free(photo_data_);
}