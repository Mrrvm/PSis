#include "lib.h"

int gallery_connect(char *host, in_port_t port) {
	
	struct sockaddr_in peer_addr;
    struct sockaddr_in gateway_addr;
    int sock_peer = 0;
    int sock_gw = 0;
    uint16_t request;
    long int ret;
    int reuse_socket = 1;

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
    		setsockopt(sock_peer, SOL_SOCKET, SO_REUSEADDR, &reuse_socket, sizeof(int));
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
	
	photo_data photo_data_;	
	FILE *photo;
	int photo_size, size_buff;
	int res;
	int type = ADD_PHOTO;
	int ret;

	// Sends type of data
	send(peer_socket, &type, sizeof(type), 0);
	
	// Opens photo
	photo = fopen(file_name, "rb");
	
	// Gets the photo size
	fseek(photo, 0L, SEEK_END);
	photo_size = ftell(photo);
	fseek(photo, 0L, SEEK_SET);
	
	// Sends photo data
	snprintf(photo_data_.file_name, sizeof(photo_data_.file_name), "%s", basename(file_name));
	photo_data_.id_photo = htonl(0);
	photo_data_.photo_size = htonl(photo_size);
	bzero(photo_data_.keyword, sizeof(photo_data_.keyword));

	res = send(peer_socket, &photo_data_, sizeof(photo_data_), 0);
	if(sizeof(photo_data_) >= res && res > 0) {
		char *buffer = malloc(photo_size);
		// Sends the photo in binary
		fread(buffer, 1, photo_size, photo);
	    send(peer_socket, buffer, photo_size, 0);
		free(buffer);
	}
	fclose(photo);
	res = recv(peer_socket, &ret, sizeof(ret), 0);
	if(sizeof(res) >= res && res > 0) {
		return (uint32_t)ret;
	}
	return 0;
} 

int gallery_add_keyword(int peer_socket, uint32_t id_photo, char *keyword) {

	int type = ADD_KEYWORD;
	photo_data photo_data_;
	int res;
	int ret;

	type = htonl(type);
	send(peer_socket, &type, sizeof(type), 0);

	photo_data_.id_photo = htonl((int)id_photo);
	snprintf(photo_data_.keyword, sizeof(photo_data_.keyword), "%s", keyword);
	printf("Sending keyword: %s\n", photo_data_.keyword);
	send(peer_socket, &photo_data_, sizeof(photo_data_), 0);

	recv(peer_socket, &ret, sizeof(ret), 0);
	printf("Return value is %d\n", ntohl(ret));
	return ntohl(ret);
}

int gallery_search_photo(int peer_socket, char * keyword, uint32_t ** id_photos) {

	int type = SEARCH_PHOTO;
	char keyword_[MESSAGE_SIZE];
	int i, j, id;

	snprintf(keyword_, sizeof(keyword_), "%s", keyword);

	id_photos = calloc(100, sizeof(uint32_t));

	type = htonl(type);
	
	send(peer_socket, &type, sizeof(type), 0);
	send(peer_socket, keyword_, sizeof(keyword_), 0);
	recv(peer_socket, &i, sizeof(i), 0);
	i = ntohl(i);

	for(j=0; j != i; j++){
		recv(peer_socket, &id, sizeof(id), 0);
		id = ntohl(id);
		id_photos[j] = id;
		printf("%d\n", id_photos[j]);

	}

	for(i=0; id_photos[i] != 0; i++){
		printf("1:%d\n", id_photos[i]);
	}

	return j;

}

int gallery_delete_photo(int peer_socket, uint32_t id_photo) {

	int type = DEL_PHOTO;
	int ret = -1;
	int res;
	
	printf("Sending delete photo\n");
	type = htonl(type);
	id_photo = htonl(id_photo);
	send(peer_socket, &type, sizeof(int), 0);
	send(peer_socket, &id_photo, sizeof(id_photo), 0);

	res = recv(peer_socket, &ret, sizeof(ret), 0);
	if(sizeof(res) >= res && res > 0) {
		return ret;
	}


}

int gallery_get_photo_name(int peer_socket, uint32_t id_photo, char **photo_name) {

	int type = GET_NAME;
	int id_photo_ = (int) id_photo;
	int res;
	char buffer[MESSAGE_SIZE];

	type = htonl(type);
	send(peer_socket, &type, sizeof(type), 0);
	id_photo_ = htonl(id_photo_);
	send(peer_socket, &id_photo_, sizeof(id_photo_), 0);

	res = recv(peer_socket, buffer, sizeof(buffer), 0);
	if(sizeof(buffer) >= res && res > 0) {
		if(strncmp(buffer, "\0", 1) != 0) {
    		*photo_name = malloc(strlen(buffer));
    		memcpy(*photo_name, buffer, strlen(buffer));
			return 1;
		}
		return 0;
	}
	else {
		return -1;
	}
}

int gallery_get_photo(int peer_socket, uint32_t id_photo, char *file_name) {
	
	int type = GET_PHOTO;
	int id_photo_ = (int) id_photo;
	int photo_size;
	int res;
	char *buffer;
	FILE *photo;

	type = htonl(type);
	send(peer_socket, &type, sizeof(type), 0);
	id_photo = htonl(id_photo);
	send(peer_socket, &id_photo, sizeof(id_photo), 0);

	res = recv(peer_socket, &photo_size, sizeof(int), 0);
	if(sizeof(int) >= res && res > 0) {
		photo_size = ntohl(photo_size);
		if(photo_size == 0) {
			// There is no photo with that id
			return 0;
		}
		
		printf("Receiving photo: %s with size %d\n", file_name, photo_size);

		buffer = malloc(photo_size);
		res = recv(peer_socket, buffer, photo_size, 0);
		if(photo_size >= res && res > 0) {
		    photo = fopen(file_name, "wb");
		    if(photo_size != fwrite(buffer, 1, photo_size, photo)) {
		        fclose(photo);
		        remove(file_name);
		        return -1;
		    }
		    fclose(photo); 
		    return 1;
		}
		free(buffer);
	}
	return -1;
}