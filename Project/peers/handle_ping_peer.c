#include "peer.h"


void *handle_ping_peer(void * arg){


	struct sockaddr_in gateway_addr_ping;
	int buff;
    int sock_gw_ping = *(int*) arg;
    int size_received;



    while(1){
    	size_received = recv(sock_gw_ping, &buff, sizeof(buff), 0);
        printf("%d\n", size_received);
    }




}



    