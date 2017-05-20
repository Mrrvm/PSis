#include "peer.h"

void handle_rep(int socket) {

    photo_data *photo_data_ = NULL;
    int photo_size = 0;
    char *buffer;
    FILE *photo;
    int res;

    photo_data_ = malloc(sizeof(photo_data));
    
    while(1) {
        res = recv(socket, photo_data_, sizeof(*photo_data_), 0);
        if(sizeof(*photo_data_) >= res && res > 0) {
            
            photo_data_->type = ntohs(photo_data_->type);
            
            if(photo_data_->type == ADD_PHOTO) {
                
                res = recv(socket, &photo_size, sizeof(photo_size), 0);
                if(sizeof(photo_size) >= res && res > 0) {

                    printf("%d\n", errno);
                    photo_size = ntohl(photo_size);
                	printf("Received photo of size %d!\n", photo_size);
                    getchar();
                    buffer = calloc(1, photo_size);
                    if(photo_size >= recv(socket, buffer, photo_size, 0)) {

                        photo = fopen("nude_received.jpg", "wb");
                        fwrite(buffer, 1, photo_size, photo);
                        fclose(photo);
                        //free(buffer); ???????????
                    }
                }
                
            }
        }
    }
}