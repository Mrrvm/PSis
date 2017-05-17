#include "gateway.h"

void *handle_cli_requests(void * arg) {

	struct sockaddr_in local_addr;
	struct sockaddr_in cli_addr;
	peer_data peer_data_;
	socklen_t size_addr;
	int sock_cli;
    uint16_t request;

    list *servers_list = (list *)arg;
    node *curr_node = NULL;

    printf(KBLU"Thread handle clients:\n"RESET);


	// Creates socket for client requests
    sock_cli = socket(AF_INET, SOCK_DGRAM, 0);
    local_addr.sin_family = AF_INET;
    local_addr.sin_port= htons(3000);
    local_addr.sin_addr.s_addr = INADDR_ANY;
    bind(sock_cli, (struct sockaddr *)&local_addr, sizeof(local_addr));
    while(1) {
    	// Waits for client requests
    	printf(KBLU"Waiting for client requests...\n"RESET);
    	size_addr = sizeof(cli_addr);
    	recvfrom(sock_cli, &request, sizeof(request), 0, 
              (struct sockaddr *) &cli_addr, &size_addr);

    	request = ntohs(request);
    	if(request == 1) {
    		printf(KBLU"Request received! Sending peer...\n"RESET);
    		if (curr_node == NULL) {
                curr_node = get_head(servers_list);
            }
            if(curr_node != NULL){
                peer_data_ = *(struct peer_data *)get_node_item(curr_node);
        		sendto(sock_cli, &peer_data_, sizeof(peer_data_), 0,
                    (const struct sockaddr *) &cli_addr, 
                    sizeof(cli_addr));
                curr_node = get_next_node(curr_node);   
            }
            else {
                sendto(sock_cli, &peer_data_, 0, 0,   //send message with size 0 so the return value of the receive is 0.
                    (const struct sockaddr *) &cli_addr, 
                    sizeof(cli_addr));
            }
    	}
    }
}