#include "gateway.h"


void *handle_ping_gw(void * arg){


	int sock_peer_ping = 0;
	struct sockaddr_in peer_addr_ping;
	struct sockaddr_in addr;
	int sock;
	int buff = 1;
	int size_sent = 0;
	list *servers_list = (list*) arg;
	node *curr_node = NULL;
	peer_data *peer_data_ = NULL;


	// Set datagram socket with peer for pings
	sock_peer_ping = socket(AF_INET, SOCK_DGRAM, 0);
    peer_addr_ping.sin_family = AF_INET;
    peer_addr_ping.sin_port = htons(3003);
    peer_addr_ping.sin_addr.s_addr = INADDR_ANY;


    while(1){	

    	while(curr_node == NULL){
    		curr_node = get_head(servers_list);
    	}
    	size_sent = 0;
    	peer_data_ = (peer_data *)get_node_item(curr_node);
    	addr = peer_data_->peer_addr;
    	sock = peer_data_->sock_ping;
    	size_sent = sendto(sock , &buff, sizeof(buff), 0,(const struct sockaddr *) &addr, sizeof(addr));
    	printf("%d\n", size_sent);
    	sleep(2);
    	if (size_sent == -1)
    	{
    		printf("THE PEER DIED\n");
    	}
    	curr_node = get_next_node(curr_node);
    }




}