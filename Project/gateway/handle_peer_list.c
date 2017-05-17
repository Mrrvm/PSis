#include "gateway.h"

void print_server(item got_item) {
    peer_data* peer_data_ = (peer_data *) got_item;
    printf("%d", ntohs(peer_data_->port));
}

void *handle_peer_list(void * arg) {

    struct sockaddr_in local_addr;
    peer_data* peer_data_;
    int sock_local;
    list *servers_list = (list *)arg;

    printf("Thread peer list:\n");

	// Creates socket for peer entries
    sock_local = socket(AF_INET, SOCK_DGRAM, 0);
    local_addr.sin_family = AF_INET;
    local_addr.sin_port= htons(3001);
    local_addr.sin_addr.s_addr= INADDR_ANY;
    bind(sock_local, (struct sockaddr *)&local_addr, sizeof(local_addr));

    peer_data_ = malloc(sizeof(peer_data));

    while(1) {

        recv(sock_local, peer_data_, sizeof(*peer_data_), 0);

        push_item_to_list(servers_list, peer_data_);
        print_list(servers_list, print_server);

    }
}