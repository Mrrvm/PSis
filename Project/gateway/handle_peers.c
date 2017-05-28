#include "gateway.h"

void set_active(item got_item, item setting) {
    peer_data *peer_data_ = (peer_data *)got_item;
    peer_data_->active = 1;
}

void *handle_peer(void *arg) {
 
    handle_peer_arg *thread_arg = (handle_peer_arg *)arg;
    list *servers_list = (*thread_arg).servers_list;
    int peer_sock = (*thread_arg).peer_socket; 
    photo_data photo_data_;
    FILE *photo;
    node *curr_node = NULL;
    peer_data peer_data_;
    int size_buff = 0;
    int photo_size = 0;
    int item_sock = 0;
    int res;
    int type;
    int n_nodes = 0;
    int i = 0;
    int id_photo;
    char *buffer;
    char keyword[MESSAGE_SIZE];
    
    while(1) {   

        res = recv(peer_sock, &type, sizeof(int), 0);
        if(sizeof(int) >= res && res > 0) {
            
            /************* SEND DATA ****************/
            if(ntohl(type) == SEND_DATA) {
                // Send data to new peer! - MUST HAVE LOCK
                printf(KYEL"[Thread peer]"RESET" Request to send data to new-born peer!\n");
                res = recv(peer_sock, &n_nodes, sizeof(n_nodes), 0);
                if(sizeof(n_nodes) >= res && res > 0) {
                    peer_data_ = *( peer_data *)get_node_item(get_head(servers_list));
                    item_sock = peer_data_.sock_peer;
                    send(item_sock, &n_nodes, sizeof(n_nodes), 0);
                    printf(KYEL"[Thread peer]"RESET" Sent %d nodes\n", ntohl(n_nodes));
                    while(i != ntohl(n_nodes)) {
                        // Receive the information
                        res = recv(peer_sock, &photo_data_, sizeof(photo_data_), 0);
                        if(sizeof(photo_data_) >= res && res > 0) {
                            // Send new peer the existant information
                            send(item_sock, &photo_data_, sizeof(photo_data_), 0);
                            printf(KYEL"[Thread peer]"RESET" Sending photo with size %d\n", photo_data_.photo_size);
                            // Receive and send the photo
                            photo_size = ntohl(photo_data_.photo_size);
                            buffer = malloc(photo_size);
                            res = recv(peer_sock, buffer, photo_size, 0);
                            if(photo_size >= res && res > 0) {
                                send(item_sock, buffer, photo_size, 0);
                            }
                            else {break;}
                            free(buffer);
                            i++;
                        }   
                        else {
                            close(peer_sock);
                            return 0;
                        } 
                    }
                    i = 0;
                    set_item_as(get_head(servers_list), set_active, NULL);
                }
                else {break;}     
            }

            /************* ADD PHOTO ****************/
            if(ntohl(type) == ADD_PHOTO) {
                res = recv(peer_sock, &photo_data_, sizeof(photo_data_), 0);
                if(sizeof(photo_data_) >= res && res > 0) {
                    photo_size = ntohl(photo_data_.photo_size);
                    buffer = malloc(photo_size);
                    res = recv(peer_sock, buffer, photo_size, 0);
                    if(photo_size >= res && res > 0) {
                        // Manages the photo id - MUST HAVE LOCK
                        photo_data_.id_photo = htonl((*thread_arg).id_counter);
                        (*thread_arg).id_counter++;

                        // Sends to all the peers for replication!
                        curr_node = get_head(servers_list);
                        while(curr_node != NULL) {
                            peer_data_ = *(peer_data *)get_node_item(curr_node);
                            item_sock = peer_data_.sock_peer;
                            send(item_sock, &type, sizeof(int), 0);
                            send(item_sock, &photo_data_, sizeof(photo_data_), 0);
                            send(item_sock, buffer, photo_size, 0);;
                            curr_node = get_next_node(curr_node);
                        }
                        free(buffer); 
                    }
                    else {break;}
                }
                else {break;}
            }

            /************* ADD KEYWORD ****************/
            if(ntohl(type) == ADD_KEYWORD) {    
                recv(peer_sock, &id_photo, sizeof(id_photo), 0);
                recv(peer_sock, keyword, sizeof(keyword), 0);
                printf(KYEL"[Thread peer]"RESET" Redirecting keyword: %s\n", keyword);
                // Sends to all the peers for replication!
                curr_node = get_head(servers_list);
                while(curr_node != NULL) {
                    peer_data_ = *(peer_data *)get_node_item(curr_node);
                    item_sock = peer_data_.sock_peer;
                    send(item_sock, &type, sizeof(int), 0);
                    send(item_sock, &id_photo, sizeof(id_photo), 0);
                    send(item_sock, keyword, sizeof(keyword), 0);
                    curr_node = get_next_node(curr_node);
                }
            }
        }
        else {break;}
    }
    close(peer_sock);
    pthread_exit(arg);
}

void *handle_peers(void * arg) {

    struct sockaddr_in local_addr;
    struct sockaddr_in local_addr_st;
    struct sockaddr_in peer_addr;
    int sock_gw_ping= 0;
    int sock_local = 0;
    int sock_peer = 0;
    int sock_peer_accepted = 0;
    int reuse_socket = 1;
    int item_sock = 0;
    int error;
    int type;
    int n_nodes = 0;
    list *servers_list = (list *)arg;
    peer_data peer_data_, head_peer;
    pthread_t thr_peer;
    handle_peer_arg *thread_arg = NULL;
    node *head_server;

    // Creates datagram socket for peer entries
    sock_local = socket(AF_INET, SOCK_DGRAM, 0);
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(3001);
    local_addr.sin_addr.s_addr = INADDR_ANY;
    bind(sock_local, (struct sockaddr *)&local_addr, sizeof(local_addr));

    // Creates stream socket to handle requests from peers
    sock_peer = socket(AF_INET, SOCK_STREAM, 0);
    local_addr_st.sin_family = AF_INET;
    local_addr_st.sin_port = htons(3002);
    local_addr_st.sin_addr.s_addr = INADDR_ANY;
    setsockopt(sock_peer, SOL_SOCKET, SO_REUSEADDR, &reuse_socket, sizeof(int));
    bind(sock_peer, (struct sockaddr *)&local_addr_st, sizeof(local_addr_st));
    listen(sock_peer, 20);

    thread_arg = malloc(sizeof(handle_peer_arg));
    (*thread_arg).id_counter = 1;

    while(1) {

        printf(KYEL"[Thread peer requests]"RESET" Waiting for peers to connect...\n");
        recv(sock_local, (struct sockaddr *) &peer_addr, sizeof(peer_addr), 0);

        printf(KYEL"[Thread peer requests]"RESET" Accepting sock stream from peer\n");
        sock_peer_accepted = accept(sock_peer, NULL, NULL);

        head_server = get_head(servers_list);
        if(head_server != NULL) {
            head_peer = *(peer_data *)get_node_item(head_server);
            type = htonl(SEND_DATA);
            item_sock = head_peer.sock_peer;
            send(item_sock, &type, sizeof(int), 0);
            peer_data_.active = 0;
        }
        else {
            printf(KYEL"[Thread peer requests]"RESET" There is 0 nodes to send.\n");
            n_nodes = htonl(n_nodes);
            send(sock_peer_accepted, &n_nodes, sizeof(int), 0);
            peer_data_.active = 1;
        }
        
        peer_data_.counter = 0;
        peer_data_.sock_peer = sock_peer_accepted;
        peer_data_.peer_addr = peer_addr;
        push_item_to_list(servers_list, &peer_data_);
        print_list(servers_list, print_server);
        
        (*thread_arg).peer_socket = sock_peer_accepted;
        (*thread_arg).servers_list = servers_list;

        error = pthread_create(&thr_peer, NULL, handle_peer, (void *)thread_arg);
        if(error != 0) {
            perror("Unable to create thread to handle peers");
            exit(-1);
        }
        pthread_detach(thr_peer);
    }
}