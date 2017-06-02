#include "peer.h"

void add_keyword(item got_item, item setting) {
     photo_data *photo_data_ = (photo_data *)got_item;
     char *keyword = (char *)setting;
     strcat(photo_data_->keyword, keyword);
}

void handle_rep(int socket, list* photo_data_list) {

    int res = 0, ret = 0, type = 0, id_photo = 0, cli_sock = 0;
    int peer_pid = 0, unwritten_len = 0, photo_size = 0, n_nodes = 0;
    char photo_name[100], keyword[MESSAGE_SIZE], *buffer;
    node *curr_node = NULL, *prev_node = NULL;
    FILE *photo;
    photo_data photo_data_;

    while(1) {
        res = recv(socket, &type, sizeof(int), 0);
        if(sizeof(type) == res) {

            /************* ADD PHOTO ****************/
            if(ntohl(type) == ADD_PHOTO) {
                ret = 0;
                res = recv(socket, &photo_data_, sizeof(photo_data_), 0);
                if(sizeof(photo_data_) == res) {
                    // Add photos to list of photos
                    photo_data_.photo_size = ntohl(photo_data_.photo_size);
                    photo_data_.id_photo = ntohl(photo_data_.id_photo);
                    photo_data_.cli_sock = ntohl(photo_data_.cli_sock);
                    photo_data_.peer_pid = ntohl(photo_data_.peer_pid);
                    photo_size = photo_data_.photo_size;
                    buffer = malloc(photo_size);
                    res = recv(socket, buffer, photo_size, 0);
                    if(photo_size >= res && res > 0) {
                        printf(KYEL"[Thread rep]"RESET": ADD_PHOTO - Received photo of size %d!\n", photo_size);
                        sprintf(photo_name, "photos/id%d", photo_data_.id_photo);
                        photo = fopen(photo_name, "wb");
                        if(photo_size == fwrite(buffer, 1, photo_size, photo)) {
                            push_item_to_list(photo_data_list, &photo_data_);
                            print_list(photo_data_list, print_photo);
                            ret = photo_data_.id_photo;
                        }
                        else {
                            remove(photo_name);
                        }
                        fclose(photo);
                    } 
                    free(buffer);
                }
                else {
                    perror(KRED"[Thread rep]"RESET);
                }
                // Sends the return value to client
                if(getpid() == photo_data_.peer_pid) {
                    send(photo_data_.cli_sock, &ret, sizeof(ret), 0); 
                }
            }

            /************* ADD KEYWORD ****************/
            if(ntohl(type) == ADD_KEYWORD) {  
                ret = 0; 
                res = recv(socket, &photo_data_, sizeof(photo_data_), 0);
                if(res == sizeof(photo_data_)) {
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
                                printf(KYEL"[Thread rep]"RESET": Keyword updated to: %s\n", photo_data_.keyword);
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

            /************* DEL PHOTO ****************/
            if(ntohl(type) == DEL_PHOTO){
                ret = 0;
                res = recv(socket, &photo_data_, sizeof(photo_data_), 0);
                if(res == sizeof(photo_data_)) {
                    id_photo = ntohl(photo_data_.id_photo);
                    cli_sock = ntohl(photo_data_.cli_sock);
                    peer_pid = ntohl(photo_data_.peer_pid);
                    
                    prev_node = NULL;
                    curr_node = get_head(photo_data_list);
                    while(curr_node != NULL){
                        photo_data_ = *(photo_data *)get_node_item(curr_node);
                        if(id_photo == photo_data_.id_photo){
                            delete_node_from_list(prev_node, curr_node, photo_data_list);
                            sprintf(photo_name, "photos/id%d", photo_data_.id_photo);
                            remove(photo_name);
                            print_list(photo_data_list, print_photo);
                            ret = 1;
                            break;
                        }
                        prev_node = curr_node;
                        curr_node = get_next_node(curr_node);
                    }
                    if(ret == 1)
                        printf(KYEL"[Thread rep]"RESET": Photo Deleted\n");
                    if(ret == 0)
                        printf(KYEL"[Thread rep]"RESET": Photo not found\n");

                    if(getpid() == peer_pid) {
                        send(cli_sock, &ret, sizeof(ret), 0); 
                    }
                }
                    
            }


            /************* SEND DATA ****************/
            if(ntohl(type) == SEND_DATA) {
                // Send data to new peer!
                send(socket, &type, sizeof(int), 0);
                n_nodes = get_list_size(photo_data_list);
                n_nodes = htonl(n_nodes);
                send(socket, &n_nodes, sizeof(int), 0);

                curr_node = get_head(photo_data_list);
                while(curr_node != NULL) {
                    photo_data_ = *(photo_data *)get_node_item(curr_node);
                    photo_size = photo_data_.photo_size;
                    sprintf(photo_name, "photos/id%d", photo_data_.id_photo);
                    photo_data_.id_photo = htonl(photo_data_.id_photo);
                    photo_data_.photo_size = htonl(photo_data_.photo_size);
                    send(socket, &photo_data_, sizeof(photo_data_), 0);

                    buffer = malloc(photo_size);
                    if(buffer == NULL) {
                        printf(KYEL"[Thread rep]"RESET": Unable to alloc buffer.\n");
                        exit(-1);
                    }
                    photo = fopen(photo_name, "rb");
                    fread(buffer, 1, photo_size, photo);
                    send(socket, buffer, photo_size, 0);
                    fclose(photo);
                    free(buffer);

                    curr_node = get_next_node(curr_node);
                }
            }
        }
    }
}