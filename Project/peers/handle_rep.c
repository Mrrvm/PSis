#include "peer.h"

void handle_rep(int socket) {

    photo_data photo_data_;
    int photo_size = 0;
    char *buffer;
    FILE *photo;
    int res;
    // pthread_mutex_t lock;   
    // pthread_mutex_init(&lock, NULL);
    int c = 0;
    while(1) {
        res = recv(socket, &photo_data_, sizeof(photo_data_), 0);
        printf("1 handle_ref %d\n", res);
        if(res == 0)
            return;
        if(sizeof(photo_data_) >= res && res > 0) {
            photo_data_.type = ntohl(photo_data_.type);
            if(photo_data_.type == ADD_PHOTO) {
                printf("%s\n", photo_data_.file_name);
                res = recv(socket, &photo_size, sizeof(photo_size), 0);
                if(sizeof(photo_size) >= res && res > 0) {
                    printf("%d\n", errno);
                    photo_size = ntohl(photo_size);
                    printf("Received photo of size %d!\n", photo_size);
                    buffer = malloc(photo_size);
                    res = recv(socket, buffer, photo_size, 0);
                    if(photo_size >= res && res > 0) {
                        char nome[100];
                        sprintf(nome, "photos/nude_received_%d.jpg", c++);
                        photo = fopen(nome, "wb");
                        //pthread_mutex_lock(&lock);
                        fwrite(buffer, 1, photo_size, photo);
                        //pthread_mutex_unlock(&lock);
                        fclose(photo);
                    }
                    free(buffer); 
                }
                
            }
        }
    }
}