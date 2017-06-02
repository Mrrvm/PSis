#include "gateway.h"

void add_counter(item got_item, item got_setting) {
     peer_data *peer_data_ = (peer_data *)got_item;
     peer_data_->counter++;
}

void set_counter(item got_item, item got_setting) {
    peer_data *peer_data_ = (peer_data *)got_item;
    peer_data_->counter = *(int *) got_setting;
}

void handle_ping_gw(list *servers_list){

    int sock_peer_ping = 0;
    struct sockaddr_in peer_addr_ping;
    struct sockaddr_in addr;
    int buff = 1;
    int size_sent, size_received;
    node *curr_node = NULL, *prev_node = NULL, *aux_node =  NULL;
    peer_data peer_data_;

    // Set datagram socket with peer for pings
    sock_peer_ping = socket(AF_INET, SOCK_DGRAM, 0);

    while(1){   

        while(curr_node == NULL){
            curr_node = get_head(servers_list);
            prev_node = NULL;
        }
        peer_data_ = *(peer_data *)get_node_item(curr_node);
        addr = peer_data_.peer_addr;

        size_sent = sendto(sock_peer_ping , &buff, sizeof(buff), 0, (const struct sockaddr *) &addr, sizeof(addr));
        usleep(50000);
        size_received = recv(sock_peer_ping, &buff, sizeof(buff), MSG_DONTWAIT);

        if (size_received == -1) {
            pthread_mutex_lock(&mux_peers);
            set_item_as(curr_node, add_counter, NULL);
            pthread_mutex_unlock(&mux_peers);
        }
        else if(peer_data_.counter > 0) {
            printf("efe\n");
            pthread_mutex_lock(&mux_peers);
            set_item_as(curr_node, set_counter, 0);   
            pthread_mutex_unlock(&mux_peers);
        }
        
        if(peer_data_.counter == 3){
            printf(KRED"[Thread ping]"RESET" Dead Peer: %d\n", ntohs(addr.sin_port));
            pthread_mutex_lock(&mux_peers);
            delete_node_from_list(prev_node, curr_node, servers_list);
            print_list(servers_list, print_server);
            pthread_mutex_unlock(&mux_peers);
        }
        prev_node = curr_node;
        curr_node = get_next_node(curr_node);
    }
}