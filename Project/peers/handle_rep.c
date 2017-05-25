#include "peer.h"

void handle_rep(int socket, list* photo_data_list) {

    photo_data *photo_data_;
    int photo_size = 0;
    char *buffer;
    FILE *photo;
    int res;
    int c = 0;
    int type;
    node *curr_node;
    char photo_name[100];

    photo_data_ = malloc(sizeof(photo_data));

    while(1) {
        res = recv(socket, &type, sizeof(int), 0);
        if(sizeof(int) >= res && res > 0) {

            if(ntohl(type) == ADD_PHOTO) {
                res = recv(socket, photo_data_, sizeof(*photo_data_), 0);
                if(sizeof(*photo_data_) >= res && res > 0) {
                    // Add photos to list of photos
                    photo_data_->id_photo = ntohl(photo_data_->id_photo);
                    printf("%d\n", photo_data_->id_photo);
                    printf("%s\n", photo_data_->file_name);
                    push_item_to_list(photo_data_list, photo_data_);
                    print_list(photo_data_list, print_photo);
                    res = recv(socket, &photo_size, sizeof(photo_size), 0);
                    if(sizeof(photo_size) >= res && res > 0) {
                        photo_size = ntohl(photo_size);
                        printf("Received photo of size %d!\n", photo_size);
                        buffer = malloc(photo_size);
                        res = recv(socket, buffer, photo_size, 0);
                        if(photo_size >= res && res > 0) {
                            sprintf(photo_name, "photos/id%d", photo_data_->id_photo);
                            photo = fopen(photo_name, "wb");
                            fwrite(buffer, 1, photo_size, photo);
                            fclose(photo);
                        } 
                        else {break;}
                        free(buffer); 
                    }
                    else {break;}
                }
                else {break;}
            }

            if(ntohl(type) == SEND_DATA) {
                // Send data to new peer!
                printf("Request to send existant data to the gateway!\n");
                send(socket, &type, sizeof(int), 0);
                int n_nodes = get_list_size(photo_data_list);
                n_nodes = htonl(n_nodes);
                send(socket, &n_nodes, sizeof(int), 0);

                curr_node = get_head(photo_data_list);
                while(curr_node != NULL) {
                    photo_data_ = get_node_item(curr_node);
                    photo_data_->id_photo = htonl(photo_data_->id_photo);
                    send(socket, photo_data_, sizeof(*photo_data_), 0);
                    curr_node = get_next_node(curr_node);
                    photo_data_->id_photo = ntohl(photo_data_->id_photo);
                }
            }
        }
        else {break;}
    }
    free(photo_data_);
}