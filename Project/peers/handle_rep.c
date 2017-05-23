#include "peer.h"

void handle_rep(int socket) {

    photo_data photo_data_;
    int photo_size = 0;
    char *buffer;
    FILE *photo;
    int res;
    
    while(1) {
        res = recv(socket, &photo_data_, sizeof(photo_data_), 0);
        if(sizeof(photo_data_) >= res && res > 0) {
            
            photo_data_.type = ntohl(photo_data_.type);
            
            if(photo_data_.type == ADD_PHOTO) {
                
                res = recv(socket, &photo_size, sizeof(photo_size), 0);
                if(sizeof(photo_size) >= res && res > 0) {

                    printf("%d\n", errno);
                    photo_size = ntohl(photo_size);
                	printf("Received photo of size %d!\n", photo_size);
                    buffer = malloc(photo_size);
                    res = recv(socket, buffer, photo_size, 0);
                    if(photo_size >= res && res > 0) {

                        photo = fopen("photos/nude_received.jpg", "wb");
                        fwrite(buffer, 1, photo_size, photo);
                        fclose(photo);
                        //free(buffer); 
                    }
                }
                
            }
        }
    }
}