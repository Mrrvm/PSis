#include "defs.h"

int main(){
    
    struct sockaddr_in server_addr;
    struct sockaddr_in gateway_addr;
    int sock_stream;
    int sock_gw;

    sock_stream = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    
	// requests an address to the gateway
    sock_gw = socket(AF_INET, SOCK_DGRAM, 0);
    gateway_addr.sin_family = AF_INET;
    inet_aton(MY_IP, &gateway_addr.sin_addr);
    sendto(sock_gw, buff, strlen(buff)+1, 0, 
				(const struct sockaddr *) &gateway_addr, 
				sizeof(gateway_addr));
	
	sleep(3);
    recv(sock_gw, buff, 100, 0);

	// sends a message to the given server
    server_addr.sin_port = ; //comes in message
	inet_aton( , &server_addr.sin_addr); 

	connect(sock_stream, 
			(const struct sockaddr *) &server_addr, 
			sizeof(server_addr));

	while(1) {
		// waits a response from the server
		// prints it
		send(sock_stream, buff, strlen(buff)+1, 0);
		recv(sock_stream, buff, 100, 0);
		printf("%s\n", buff);
	}


}
