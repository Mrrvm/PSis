#include "gateway.h"

void print_server(item got_item) {
    struct sockaddr_in peer_addr = *(struct sockaddr_in *) got_item;
    printf("%d\n", ntohs(peer_addr.sin_port));
}

void *handle_peer_list(void * arg) {

    struct sockaddr_in local_addr;
    struct sockaddr_in peer_addr;
    int sock_local;


    list *servers_list = create_list(sizeof(peer_addr));
    node *curr_node = NULL;

    printf("Thread peer list:\n");

	// Creates socket for peer entries
    sock_local = socket(AF_INET, SOCK_DGRAM, 0);
    local_addr.sin_family = AF_INET;
    local_addr.sin_port= htons(3001);
    local_addr.sin_addr.s_addr= INADDR_ANY;
    bind(sock_local, (struct sockaddr *)&local_addr, sizeof(local_addr));



    while(1) {

        recv(sock_local, (struct sockaddr *) &peer_addr, sizeof(peer_addr), 0);

        push_item_to_list(servers_list, &peer_addr);
        print_list(servers_list, print_server);

    }
}