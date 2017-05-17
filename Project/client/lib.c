#include "lib.h"

int gallery_connect(char *host, in_port_t port) {
	
	struct sockaddr_in peer_addr;
	peer_data peer_data_;
    struct sockaddr_in gateway_addr;
    int sock_peer = 0;
    int sock_gw = 0;
    uint16_t request;
    long int ret;

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
	    ret = recv(sock_gw, &peer_data_, sizeof(peer_data_), 0);
	  
	    if(ret != 0 && ret != -1) {
			// Connects to the peer
	    	printf("Received IP %s:%d from Gateway\n", inet_ntoa(peer_data_.adress), ntohs(peer_data_.port));
			
	    	// Check if peer is available, if not return 0!

			// Creates connection with peer
			sock_peer = socket(AF_INET, SOCK_STREAM, 0);
			peer_addr.sin_family = AF_INET;
			if(0 == connect(sock_peer, 
				 &peer_data_, 
				sizeof(peer_data_))) {
				
				return sock_peer;
			}
			else {
				return 0;
			}
	    }
	    else if(ret==-1){
	    	return -1;
	    }
	    else if(ret==0){
	    	return 0;
	    }
    }
    else {
    	return -1;
    }
}

uint32_t gallery_add_photo(int peer_socket, char *file_name) {
	
	photo_data *photo_data_;
	FILE *photo;
	int photo_size, size_buff;

	photo_data_ = malloc(sizeof(photo_data));

	// Sends photo data
	photo_data_->type = htons(ADD_PHOTO);
	strcpy(photo_data_->file_name, file_name);
	photo_data_->id_photo = htons(0);

	send(peer_socket, photo_data_, sizeof(*photo_data_), 0);

	free(photo_data_);

	// Opens photo
	photo = fopen(file_name, "rb");
	
	// Gets the photo size
	fseek(photo, 0L, SEEK_END);
	photo_size = ftell(photo);
	fseek(photo, 0L, SEEK_SET);
	char * buffer=malloc(photo_size);
	size_buff = htonl(photo_size);

	// Sends photo size
	send(peer_socket, &size_buff, sizeof(size_buff), 0);
	
	// Sends the photo in binary
	fread(buffer, 1, photo_size, photo);
    send(peer_socket, buffer, photo_size, 0);

}	