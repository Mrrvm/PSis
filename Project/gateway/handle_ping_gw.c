#include "gateway.h"

void print_server1(item got_item) {
    peer_data *peer_data_ = (peer_data *)got_item;
    struct sockaddr_in peer_addr = peer_data_->peer_addr; 
    printf("%d", ntohs(peer_addr.sin_port));
}

void *handle_ping_gw(void * arg){


    int sock_peer_ping = 0;
    struct sockaddr_in peer_addr_ping;
    struct sockaddr_in addr;
    int buff = 1;
    int size_sent, size_received;
    list *servers_list = (list*) arg;
    node *curr_node = NULL, *prev_node = NULL, *aux_node =  NULL;
    peer_data *peer_data_ = NULL;


    // Set datagram socket with peer for pings
    sock_peer_ping = socket(AF_INET, SOCK_DGRAM, 0);


    while(1){   

        while(curr_node == NULL){
            curr_node = get_head(servers_list);
            prev_node = NULL;
        }
        peer_data_ = (peer_data *)get_node_item(curr_node);
        addr = peer_data_->peer_addr;

        size_sent = sendto(sock_peer_ping , &buff, sizeof(buff), 0, (const struct sockaddr *) &addr, sizeof(addr));
        usleep(50000);
        size_received = recv(sock_peer_ping, &buff, sizeof(buff), MSG_DONTWAIT);

        if (size_received == -1)
        {
            peer_data_->counter++;
        }

        if(peer_data_->counter == 3){
            printf("Dead Peer: %d\n", ntohs(addr.sin_port));
            aux_node = curr_node;
            if(prev_node != NULL) {
                set_next_node(prev_node, get_next_node(curr_node));
                free_node(aux_node, free);
            }
            else if(get_next_node(curr_node) != NULL) {
                set_head(servers_list, get_next_node(curr_node));
                set_next_node(curr_node, NULL);
                free_node(aux_node, free);
            }
            else {
                set_head(servers_list, NULL);
                free_node(aux_node, free);
            }
            decrement_list_size(servers_list);
            print_list(servers_list, print_server1);
        }
        prev_node = curr_node;
        curr_node = get_next_node(curr_node);
    }




}