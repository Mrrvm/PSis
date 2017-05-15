#include "gateway.h"

void *handle_cli_requests(void * arg) {

	struct sockaddr_in local_addr;
	struct sockaddr_in cli_addr;
	struct sockaddr_in peer_addr;
	socklen_t size_addr;
	int sock_cli;
    uint16_t request;

    list *servers_list = (list *)arg;
    node *curr_node = NULL;

    printf("Thread handle clients:\n");


	// Creates socket for client requests
    sock_cli = socket(AF_INET, SOCK_DGRAM, 0);
    local_addr.sin_family = AF_INET;
    local_addr.sin_port= htons(3000);
    local_addr.sin_addr.s_addr = INADDR_ANY;
    bind(sock_cli, (struct sockaddr *)&local_addr, sizeof(local_addr));
    while(1) {
    	// Waits for client requests
    	printf("\nWaiting for client requests...\n");
    	size_addr = sizeof(cli_addr);
    	recvfrom(sock_cli, &request, sizeof(request), 0, 
              (struct sockaddr *) &cli_addr, &size_addr);

    	request = ntohs(request);
    	if(request == 1) {
    		printf("Request received! Sending peer...\n");
    		if (curr_node == NULL) {
                curr_node = get_head(servers_list);
            }
            if(curr_node != NULL){
                peer_addr = *(struct sockaddr_in *)get_node_item(curr_node);
        		sendto(sock_cli,(const struct sockaddr *) &peer_addr, sizeof(peer_addr), 0,
                    (const struct sockaddr *) &cli_addr, 
                    sizeof(cli_addr));
                curr_node = get_next_node(curr_node);   
            }
            else {
                printf("List is empty\n");
            }
    	}
    }
}