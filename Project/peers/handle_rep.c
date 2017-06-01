#include "peer.h"

void add_keyword(item got_item, item setting) {
     photo_data *photo_data_ = (photo_data *)got_item;
     char *keyword = (char *)setting;
     strcat(photo_data_->keyword, keyword);
}

void handle_rep(int socket, list* photo_data_list) {

    photo_data photo_data_;
    int photo_size = 0;
    char *buffer;
    FILE *photo;
    int res;
    int ret;
    int type;
    int id_photo;
    char keyword[MESSAGE_SIZE];
    node *curr_node;
    char photo_name[100];
    int unwritten_len;

    while(1) {
        res = recv(socket, &type, sizeof(int), 0);
        if(sizeof(int) >= res && res > 0) {

            /************* ADD PHOTO ****************/
            if(ntohl(type) == ADD_PHOTO) {
                ret = 0;
                res = recv(socket, &photo_data_, sizeof(photo_data_), 0);
                if(sizeof(photo_data_) >= res && res > 0) {
                    // Add photos to list of photos
                    photo_data_.photo_size = ntohl(photo_data_.photo_size);
                    photo_data_.id_photo = ntohl(photo_data_.id_photo);
                    photo_data_.cli_sock = ntohl(photo_data_.cli_sock);
                    photo_data_.peer_pid = ntohl(photo_data_.peer_pid);
                    photo_size = photo_data_.photo_size;
                    buffer = malloc(photo_size);
                    res = recv(socket, buffer, photo_size, 0);
                    if(photo_size >= res && res > 0) {
                        printf(KYEL"[Thread rep]"RESET" Received photo of size %d!\n", photo_size);
                        sprintf(photo_name, "photos/id%d", photo_data_.id_photo);
                        photo = fopen(photo_name, "wb");
                        if(photo_size == fwrite(buffer, 1, photo_size, photo)) {
                            push_item_to_list(photo_data_list, &photo_data_);
                            print_list(photo_data_list, print_photo);
                            fclose(photo);
                            ret = photo_data_.id_photo;
                        }
                        else {
                            fclose(photo);
                            remove(photo_name);
                        }
                    } 
                    free(buffer);
                }
                if(getpid() == photo_data_.peer_pid) {
                    send(photo_data_.cli_sock, &ret, sizeof(ret), 0); 
                }
            }

            /************* SEND DATA ****************/
            if(ntohl(type) == SEND_DATA) {
                // Send data to new peer!
                printf(KYEL"[Thread rep]"RESET" Request to send existant data to the gateway!\n");
                send(socket, &type, sizeof(int), 0);
                int n_nodes = get_list_size(photo_data_list);
                n_nodes = htonl(n_nodes);
                send(socket, &n_nodes, sizeof(int), 0);

                curr_node = get_head(photo_data_list);
                while(curr_node != NULL) {
                    photo_data_ = *(photo_data *)get_node_item(curr_node);
                    photo_size = photo_data_.photo_size;
                    sprintf(photo_name, "photos/id%d", photo_data_.id_photo);

                    printf(KYEL"[Thread rep]"RESET" Sending photo with size %d\n", photo_data_.photo_size);
                    photo_data_.id_photo = htonl(photo_data_.id_photo);
                    photo_data_.photo_size = htonl(photo_data_.photo_size);
                    send(socket, &photo_data_, sizeof(photo_data_), 0);

                    char *buffer = malloc(photo_size);
                    photo = fopen(photo_name, "rb");
                    fread(buffer, 1, photo_size, photo);
                    send(socket, buffer, photo_size, 0);
                    fclose(photo);
                    free(buffer);

                    curr_node = get_next_node(curr_node);
                }
            }

            /************* ADD KEYWORD ****************/
            if(ntohl(type) == ADD_KEYWORD) {  
                ret = 0; 
                recv(socket, &photo_data_, sizeof(photo_data_), 0);
                photo_data_.cli_sock = ntohl(photo_data_.cli_sock);
                photo_data_.peer_pid = ntohl(photo_data_.peer_pid);
                id_photo = ntohl(photo_data_.id_photo);
                snprintf(keyword, sizeof(keyword), "%s", photo_data_.keyword);
                curr_node = get_head(photo_data_list);
                while(curr_node != NULL) {
                    photo_data_ = *(photo_data *)get_node_item(curr_node);
                    if(photo_data_.id_photo == id_photo) {
                        unwritten_len = MESSAGE_SIZE-strlen(photo_data_.keyword);
                        if(unwritten_len > strlen(keyword)+1) {
                            set_item_as(curr_node, add_keyword, keyword);
                            photo_data_ = *(photo_data *)get_node_item(curr_node);
                            printf(KYEL"[Thread rep]"RESET" Keyword updated to: %s\n", photo_data_.keyword);
                            ret = 1;
                        }
                        break;
                    }
                    curr_node = get_next_node(curr_node);
                }
                if(getpid() == photo_data_.peer_pid) {
                    ret = ntohl(ret);
                    send(photo_data_.cli_sock, &ret, sizeof(ret), 0); 
                }

            }
        }
    }
}