#include "peer.h"


void *handle_ping_peer(void * arg){


	struct sockaddr_in gateway_addr_ping;
	int buff;
    int sock_gw_ping = *(int*) arg;
    int size_received, size_sent;
    int addr_size;



    while(1){
        addr_size = sizeof(gateway_addr_ping);
    	size_received = recvfrom(sock_gw_ping, &buff, sizeof(buff), 0, (struct sockaddr *) &gateway_addr_ping, &addr_size);
        size_sent = size_sent = sendto(sock_gw_ping, &buff, sizeof(buff), 0, (const struct sockaddr*)  &gateway_addr_ping, sizeof(gateway_addr_ping));
        printf("RECEIVED: %d  SENT: %d\n", size_received, size_sent);
    }




}



    