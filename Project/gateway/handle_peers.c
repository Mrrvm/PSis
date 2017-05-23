#include "gateway.h"

void print_server(item got_item) {
    peer_data *peer_data_ = (peer_data *)got_item;
    struct sockaddr_in peer_addr = peer_data_->peer_addr; 
    printf("%d", ntohs(peer_addr.sin_port));
}

void *handle_peer(void *arg) {
 
    handle_peer_arg *thread_arg = (handle_peer_arg *)arg;
    list *servers_list = (*thread_arg).servers_list;
    int peer_sock = (*thread_arg).peer_socket; 
    photo_data photo_data_;
    int photo_size = 0;
    //char *buffer;
    FILE *photo;
    int size_buff = 0;
    node *curr_node = NULL;
    int item_sock = 0;
    peer_data *peer_data_;
    int res;
    
    while(1) {    
        char *buffer;
        res = recv(peer_sock, &photo_data_, sizeof(photo_data_), 0);
        if (res<=0){
            printf("PEER ERROR\n");
            break;
        }
        else if(sizeof(photo_data_) >= res && res > 0) {
            
            if(ntohl(photo_data_.type) == ADD_PHOTO) {

                res = recv(peer_sock, &photo_size, sizeof(photo_size), 0);
                if (res<=0){
                    printf("PEER ERROR\n");
                    break;
                }
                else if(sizeof(photo_size) >= res && res > 0) {
                    size_buff = ntohl(photo_size);
                    printf("size_buff %d photo_size %d\n", size_buff, photo_size );
                    printf("Received photo size: %d\n", size_buff);
                    buffer = malloc(size_buff);
                    recv(peer_sock, buffer, size_buff, 0);
                }
            }

            // Sends to all the peers for replication!
            curr_node = get_head(servers_list);
            while(curr_node != NULL) {
                peer_data_ = get_node_item(curr_node);
                item_sock = peer_data_->sock_peer;
                res = send(item_sock, &photo_data_, sizeof(photo_data_), 0);
                if(sizeof(photo_data_) >= res && res > 0) {
                    res = send(item_sock, &photo_size, sizeof(photo_size), 0);
                    if(sizeof(photo_size) >= res && res > 0) {
                       res = send(item_sock, buffer, size_buff, 0);
                       printf("sent %d\n", res);
                    }
                }
                curr_node = get_next_node(curr_node);
            }

            //free(buffer); 
        }
    }
    close(peer_sock);
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
    int error;
    list *servers_list = (list *)arg;
    peer_data *peer_data_;
    pthread_t thr_peer;
    handle_peer_arg *thread_arg = NULL;

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

    peer_data_ = malloc(sizeof(peer_data));
    thread_arg = malloc(sizeof(handle_peer_arg));

    while(1) {

        printf(KYEL"[Thread peer requests]"RESET" Waiting for peers to connect...\n");
        recv(sock_local, (struct sockaddr *) &peer_addr, sizeof(peer_addr), 0);

        //recv(sock_local, &sock_gw_ping, sizeof(sock_gw_ping), 0);

        printf(KYEL"[Thread peer requests]"RESET" Accepting sock stream from peer\n");
        sock_peer_accepted = accept(sock_peer, NULL, NULL);
        
        //peer_data_->sock_ping = sock_gw_ping;
        peer_data_->sock_peer = sock_peer_accepted;
        peer_data_->peer_addr = peer_addr;
        push_item_to_list(servers_list, peer_data_);
        print_list(servers_list, print_server);
        
        (*thread_arg).peer_socket = sock_peer_accepted;
        (*thread_arg).servers_list = servers_list;

        error = pthread_create(&thr_peer, NULL, handle_peer, (void *)thread_arg);
        if(error != 0) {
            perror("Unable to create thread to handle peers");
            exit(-1);
        }

    }
}