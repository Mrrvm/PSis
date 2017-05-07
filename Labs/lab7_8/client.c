#include "defs.h"

int main(){
    
    struct sockaddr_in server_addr;
    struct sockaddr_in gateway_addr;
    int sock_stream;
    int sock_gw;
    message_gw *message_gw_;
    message_std *message_std_;

    // gateway settings
    sock_gw = socket(AF_INET, SOCK_DGRAM, 0);
    gateway_addr.sin_family = AF_INET;
    gateway_addr.sin_port = htons(3000);
    inet_aton(GW_IP, &gateway_addr.sin_addr);

	// requests an address to the gateway
	message_gw_ = malloc(sizeof(message_gw));
	message_gw_->type = CLI_GW;
    sendto(sock_gw, message_gw_, sizeof(*message_gw_), 0, 
				(const struct sockaddr *) &gateway_addr, 
				sizeof(gateway_addr));
	
	// receives a server address
    recv(sock_gw, message_gw_, sizeof(*message_gw_), 0);
    spam(("Received IP from gw\n"));
    // connects to that server via stream
    sock_stream = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(message_gw_->port);
	inet_aton(message_gw_->address, &server_addr.sin_addr); 
	if(0 == connect(sock_stream, 
			(const struct sockaddr *) &server_addr, 
			sizeof(server_addr)))
	{	
		// gets message to send
		message_std_ = malloc(sizeof(message_std));

		printf("\nWrite message:\n");
		fgets(message_std_->buffer, MESSAGE_LEN, stdin);
		// sends a message to the given server
		send(sock_stream, message_std_, sizeof(*message_std_), 0);
		// waits a response from the server
		recv(sock_stream, message_std_, sizeof(*message_std_), 0);
		// prints it
		printf("\nThe server said: %s\n", message_std_->buffer);
	}
	else {
		printf("Could not connect\n");
		exit(0);
	}
	close(sock_stream);
}
