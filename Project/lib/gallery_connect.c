#include "lib.h"

int gallery_connect(char *host, in_port_t port) {
	
	struct sockaddr_in peer_addr;
    struct sockaddr_in gateway_addr;
    int sock_peer = 0;
    int sock_gw = 0;
    uint16_t request;
    long int ret = sizeof(peer_addr);
    int peer_port;
    char *peer_address;

    // Gateway settings
    sock_gw = socket(AF_INET, SOCK_DGRAM, 0);
    gateway_addr.sin_family = AF_INET;
    gateway_addr.sin_port = htons(port);
    inet_aton(host, &gateway_addr.sin_addr);

    request = htons(1);
 	// Send the request to the gateway
    if(-1 != sendto(sock_gw, &request, sizeof(request), 0, 
				(const struct sockaddr *) &gateway_addr, 
				sizeof(gateway_addr))) {
	    // Receives a server address
	    ret = recv(sock_gw, &peer_addr, sizeof(peer_addr), 0);
	  
	    if(ret != 0 && ret != -1) {
			// Connects to the peer
			peer_port = ntohs(peer_addr.sin_port);
			peer_address = inet_ntoa(peer_addr.sin_addr);
	    	printf("Received IP %s:%d from Gateway\n", peer_address, peer_port);
			
			// Creates connection with peer
			return sock_peer;
	    }	
    }
    return 0;
}