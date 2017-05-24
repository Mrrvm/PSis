#include "lib.h"

int gallery_connect(char *host, in_port_t port) {
	
	struct sockaddr_in peer_addr;
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
	    ret = recv(sock_gw, &peer_addr, sizeof(peer_addr), 0);
	  
	    if(ret != 0 && ret != -1) {
			// Connects to the peer
	    	printf("Received IP %s:%d from Gateway\n", inet_ntoa(peer_addr.sin_addr), ntohs(peer_addr.sin_port));
			
			// Creates connection with peer
			sock_peer = socket(AF_INET, SOCK_STREAM, 0);
			if(0 == connect(sock_peer, 
				(const struct sockaddr *) &peer_addr, 
				sizeof(peer_addr))) {
				
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
	int res;
	int type = ADD_PHOTO;

	photo_data_ = malloc(sizeof(photo_data));
	// Sends type of data
	send(peer_socket, &type, sizeof(int), 0);
	// Sends photo data
	strcpy(photo_data_->file_name, file_name);
	photo_data_->id_photo = htonl(0);
	res = send(peer_socket, photo_data_, sizeof(*photo_data_), 0);
	if(sizeof(*photo_data_) >= res && res > 0) {
		// Opens photo
		photo = fopen(file_name, "rb");
		// Gets the photo size
		fseek(photo, 0L, SEEK_END);
		photo_size = ftell(photo);
		fseek(photo, 0L, SEEK_SET);
		char *buffer= malloc(photo_size);
		size_buff = htonl(photo_size);
		// Sends photo size
		res = send(peer_socket, &size_buff, sizeof(size_buff), 0);
		if(sizeof(size_buff) >= res && res > 0) {
			// Sends the photo in binary
			fread(buffer, 1, photo_size, photo);
		    send(peer_socket, buffer, photo_size, 0);
		}
		free(buffer);
	}
	free(photo_data_);
} 

int gallery_add_keyword(int peer_socket, uint32_t id_photo, char *keyword) {

}

int gallery_search_photo(int peer_socket, char * keyword, uint32_t ** id_photos) {

}

int gallery_delete_photo(int peer_socket, uint32_t id_photo) {

}

int gallery_get_photo_name(int peer_socket, uint32_t id_photo, char **photo_name) {

}

int gallery_get_photo(int peer_socket, uint32_t id_photo, char *file_name) {
	
}