#include "lib.h"

/************************************************************************
 gallery_connect: Connects the client with a peer.
 Returns:
 	ret - Positive integer icorresponding to the opened socket
 	0 - No peer is available.
 	-1 - Gateway can not be accessed
*************************************************************************/
int gallery_connect(char *host, in_port_t port) {
	
	struct sockaddr_in peer_addr;
    struct sockaddr_in gateway_addr;
    int sock_peer = 0, sock_gw = 0;
    uint16_t request;
    int ret;
    int reuse_socket = 1;

    // Gateway settings
    sock_gw = socket(AF_INET, SOCK_DGRAM, 0);
    gateway_addr.sin_family = AF_INET;
    gateway_addr.sin_port = htons(port);
    inet_aton(host, &gateway_addr.sin_addr);

    request = htons(1);
 	// Send the request to the gateway
    if(-1 != sendto(sock_gw, &request, sizeof(request), 0, (const struct sockaddr *) &gateway_addr, sizeof(gateway_addr))) {
	    // Receives a server address
	    ret = recv(sock_gw, &peer_addr, sizeof(peer_addr), 0);
	    if(ret != 0 && ret != -1) {
			// Connects to the peer
	    	printf("Received IP %s:%d from Gateway\n", inet_ntoa(peer_addr.sin_addr), ntohs(peer_addr.sin_port));
			// Creates connection with peer
			sock_peer = socket(AF_INET, SOCK_STREAM, 0);
    		setsockopt(sock_peer, SOL_SOCKET, SO_REUSEADDR, &reuse_socket, sizeof(int));
			if(0 == connect(sock_peer, (const struct sockaddr *) &peer_addr, sizeof(peer_addr))) {
				return sock_peer;
			}
			else {
				perror(KRED"[gallery_connect]"RESET);
				return 0;
			}
	    }
	    else if(ret == -1){
	    	perror(KRED"[gallery_connect]"RESET);
	    	return -1;
	    }
	    else if(ret == 0){
	    	perror(KRED"[gallery_connect]"RESET);
	    	return 0;
	    }
    }
    else {
    	perror(KRED"[gallery_connect]"RESET);
    	return -1;
    }
}

/************************************************************************
 gallery_add_photo: Adds a photo to the system.
 Returns:
 	0 - Invalid filename, problems in communication, photo not added, ...
 	id_photo - Positive integer (corresponding to the photo identifier)
*************************************************************************/
uint32_t gallery_add_photo(int peer_socket, char *file_name) {
	
	int type = ADD_PHOTO;
	int res = 0, ret = 0, photo_size = 0;
	photo_data photo_data_;	
	FILE *photo;
	char *buffer;

	// Sends type of data
	send(peer_socket, &type, sizeof(type), 0);
	// Opens photo
	photo = fopen(file_name, "rb");
	// Gets the photo size
	fseek(photo, 0L, SEEK_END);
	photo_size = ftell(photo);
	fseek(photo, 0L, SEEK_SET);

	// Sends photo data in network format
	if(snprintf(photo_data_.file_name, sizeof(photo_data_.file_name), "%s", basename(file_name)) 
		!= strlen(photo_data_.file_name)) {
		printf(KRED"[gallery_add_photo]"RESET": Invalid file_name\n");
		return 0;
	}
	photo_data_.id_photo = htonl(0);
	photo_data_.photo_size = htonl(photo_size);
	bzero(photo_data_.keyword, sizeof(photo_data_.keyword));
	if(-1 == send(peer_socket, &photo_data_, sizeof(photo_data_), 0)) {
		perror(KRED"[gallery_add_photo]"RESET);
		return 0;
	}

	// Sends the photo in binary
	buffer = malloc(photo_size);
	fread(buffer, 1, photo_size, photo);
    if(-1 == send(peer_socket, buffer, photo_size, 0)) {
    	perror(KRED"[gallery_add_photo]"RESET);
    	return 0;
    }
	free(buffer);
	fclose(photo);

	// Receives the result from adding photo
	res = recv(peer_socket, &ret, sizeof(ret), 0);
	if(sizeof(ret) == res) {
		return (uint32_t)ret;
	}
	perror(KRED"[gallery_add_photo]"RESET);
	return 0;
} 


/************************************************************************
 gallery_add_keyword: Adds keyword to an existant photo.
 Returns:
 	1 - Keyword was addded to the photo
 	0 - Photo does not exist
 	-1 - Invalid arguments, problems in communication, ...
*************************************************************************/
int gallery_add_keyword(int peer_socket, uint32_t id_photo, char *keyword) {

	int type = ADD_KEYWORD;
	int res = 0, ret = 0;
	photo_data photo_data_;

	type = htonl(type);
	send(peer_socket, &type, sizeof(type), 0);

	photo_data_.id_photo = htonl((int)id_photo);
	if(snprintf(photo_data_.keyword, sizeof(photo_data_.keyword), "%s", keyword)
		!= strlen(photo_data_.keyword)) {
		printf(KRED"[gallery_add_keyword]"RESET": Invalid keyword\n");
		return -1;
	}
	
	send(peer_socket, &photo_data_, sizeof(photo_data_), 0);

	res = recv(peer_socket, &ret, sizeof(ret), 0);
	if(res == sizeof(ret)) {
		return ntohl(ret);
	}
	return -1;
}

/************************************************************************
 gallery_search_photo: Searches for photo by keyword.
 Returns:
 	num_found - positive integer (corresponding to number of found photos)
 	0 - No photo contains the provided keyword
 	-1 - Invalid arguments, problems in communication, ...
*************************************************************************/
int gallery_search_photo(int peer_socket, char *keyword, uint32_t **id_photos) {

	int type = SEARCH_PHOTO;
	char keyword_[MESSAGE_SIZE];
	int num_found = 0, j = 0, id = 0, res = 0;

	if(snprintf(keyword_, sizeof(keyword_), "%s", keyword)!= strlen(keyword_)) {
		printf(KRED"[gallery_search_photo]"RESET": Invalid keyword\n");
		return -1;
	}

	type = htonl(type);
	send(peer_socket, &type, sizeof(type), 0);
	send(peer_socket, keyword_, sizeof(keyword_), 0);
	
	res = recv(peer_socket, &num_found, sizeof(num_found), 0);
	if(res == sizeof(num_found)) {
		num_found = ntohl(num_found);
		if(num_found == -1) {
			return -1;
		}
		for(j=0; j != num_found; j++) {
			recv(peer_socket, &id, sizeof(id), 0);
			id_photos[j] = (uint32_t) ntohl(id);
		}
		return num_found;
	}
	return -1;
}


/************************************************************************
 gallery_delete_photo: Removes photo from the system.
 Returns:
 	1 - Photo is removed successfully
 	0 - Photo does not exist
 	-1 - Invalid arguments, problems in communication, ...
*************************************************************************/
int gallery_delete_photo(int peer_socket, uint32_t id_photo) {

	int type = DEL_PHOTO;
	int ret = 0, res = 0;
	
	type = htonl(type);
	id_photo = htonl(id_photo);
	send(peer_socket, &type, sizeof(int), 0);
	send(peer_socket, &id_photo, sizeof(id_photo), 0);

	res = recv(peer_socket, &ret, sizeof(ret), 0);
	if(sizeof(ret) == res) {
		return ret;
	}
	return -1;
}

/************************************************************************
 gallery_get_photo_name: Retrieves photo name from the system by photo id.
 Returns:
 	1 - Photo exists in the system and the name was retrieved
 	0 - Photo does not exist
 	-1 - Invalid arguments, problems in communication, ...
*************************************************************************/
int gallery_get_photo_name(int peer_socket, uint32_t id_photo, char **photo_name) {

	int type = GET_NAME;
	int id_photo_ = (int) id_photo;
	int res = 0;
	char name[MESSAGE_SIZE];

	type = htonl(type);
	send(peer_socket, &type, sizeof(type), 0);
	id_photo_ = htonl(id_photo_);
	send(peer_socket, &id_photo_, sizeof(id_photo_), 0);

	res = recv(peer_socket, name, sizeof(name), 0);
	if(sizeof(name) == res) {
		// If the photo does not exist, name is null
		if(strncmp(name, "\0", 1) != 0) {
    		*photo_name = malloc(strlen(name));
    		memcpy(*photo_name, name, strlen(name));
			return 1;
		}
		return 0;
	}
	else {
		return -1;
	}
}

/************************************************************************
 gallery_get_photo: Retrieves photo from the system by photo id.
 Returns:
 	1 - Photo is downloaded successfully
 	0 - Photo does not exist
 	-1 - Invalid arguments, problems in communication, ...
*************************************************************************/
int gallery_get_photo(int peer_socket, uint32_t id_photo, char *file_name) {
	
	int type = GET_PHOTO;
	int id_photo_ = (int) id_photo;
	int photo_size = 0, res = 0;
	char *buffer;
	FILE *photo;

	type = htonl(type);
	send(peer_socket, &type, sizeof(type), 0);
	id_photo = htonl(id_photo);
	send(peer_socket, &id_photo, sizeof(id_photo), 0);

	res = recv(peer_socket, &photo_size, sizeof(int), 0);
	if(sizeof(photo_size) == res) {
		photo_size = ntohl(photo_size);
		if(photo_size == 0) {
			// Photo does not exist
			return 0;
		}
		buffer = malloc(photo_size);
		int n = 0;
		while(n != photo_size){
			res = recv(peer_socket, buffer+n, photo_size-n, 0);
			n += res;
		}

		printf("n: %d\n", n);
		printf("Size received: %d\n", res);

		if(photo_size == n) {
		    photo = fopen(file_name, "wb");
		    if(photo_size != fwrite(buffer, 1, photo_size, photo)) {
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