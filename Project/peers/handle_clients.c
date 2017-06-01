#include "peer.h"

void *handle_client(void * arg) {

	handle_client_arg *thread_arg = (handle_client_arg *)arg;
	int client_socket = (*thread_arg).client_sock;
	int gw_socket = (*thread_arg).gw_sock;
	int res = 0, ret = 0, type = 0, id_photo = 0, photo_size = 0, i = 0, j = 0;
	int id_array[100] = {0};
	char photo_name[100], keyword[MESSAGE_SIZE], *buffer;
	node *curr_node = NULL;
	list *photo_data_list = (*thread_arg).photo_data_list;
	photo_data photo_data_, photo_data_aux;
	FILE *photo;

	printf(KBLU"[Thread client]"RESET": Handling client\n");

	while(1) {
		// Receives type and executes it
		res = recv(client_socket, &type, sizeof(int), 0);
		if(sizeof(type) == res) {
			
			/************* ADD PHOTO ****************/
			if(ntohl(type) == ADD_PHOTO) {
				// Sends type to the gateway
				res = recv(client_socket, &photo_data_, sizeof(photo_data_), 0);
				if(sizeof(photo_data_) == res) {
					// Receives and sends photo and photo data
					photo_size = ntohl(photo_data_.photo_size);			
					buffer = malloc(photo_size);
					if(buffer == NULL){
						printf(KBLU"[Thread client]"RESET": Unable to alloc buffer\n");
						exit(0);
					}
					res = recv(client_socket, buffer, photo_size, 0);
					if(photo_size == res) {
						printf(KBLU"[Thread client]"RESET": Redirecting photo of size %d\n", photo_size);
						// Sends type and photo data to the gateway
						photo_data_.cli_sock = htonl(client_socket);
						photo_data_.peer_pid = htonl(getpid());
						send(gw_socket, &type, sizeof(int), 0);
						send(gw_socket, &photo_data_, sizeof(photo_data_), 0);
						send(gw_socket, buffer, photo_size, 0);
						free(buffer);
					}
					else {
						ret = -1;
						send(client_socket, &ret, sizeof(ret), 0);
						perror(KRED"[Thread client]"RESET);
						break;
					}
				}				
				else {
					ret = -1;
					send(client_socket, &ret, sizeof(ret), 0);
					perror(KRED"[Thread client]"RESET);
					break;
				}
			}

			/************* ADD KEYWORD ****************/
			if(ntohl(type) == ADD_KEYWORD) {
				res = recv(client_socket, &photo_data_aux, sizeof(photo_data_aux), 0);
				if(sizeof(photo_data_) == res) {
					id_photo = ntohl(photo_data_aux.id_photo);
					snprintf(keyword, sizeof(keyword), "%s", photo_data_aux.keyword);
					// Checks if keyword already exists in the photo
					curr_node = get_head(photo_data_list);
					while(curr_node != NULL) {
						photo_data_ = *(photo_data *)get_node_item(curr_node);
						if(photo_data_.id_photo == id_photo) {
							if(NULL != strstr(photo_data_.keyword, keyword)) {
								ret = 1;
							}
							else {
								send(gw_socket, &type, sizeof(int), 0);
								photo_data_aux.cli_sock = htonl(client_socket);
								photo_data_aux.peer_pid = htonl(getpid());
								send(gw_socket, &photo_data_aux, sizeof(photo_data_aux), 0);
								printf(KBLU"[Thread client]"RESET": Redirecting keyword: %s\n", photo_data_aux.keyword);
							}
							break;
						}
						curr_node = get_next_node(curr_node);
					}
					// Did not find the photo
					if(curr_node == NULL) {
						ret = 0;
						ret = htonl(ret);
						send(client_socket, &ret, sizeof(ret), 0);
					}

				}
				else {
					ret = -1;
					ret = htonl(ret);
					send(client_socket, &ret, sizeof(ret), 0);
				}
			}

			/************* SEARCH PHOTO ****************/
			if(ntohl(type) == SEARCH_PHOTO) {
				recv(client_socket, keyword, sizeof(keyword), 0);
				printf(KBLU"[Thread client]"RESET": Request to search photo %s\n", keyword);

				i=0;
				curr_node = get_head(photo_data_list);
				while(curr_node != NULL) {
					photo_data_ = *(photo_data *)get_node_item(curr_node);
					if(NULL != strstr(photo_data_.keyword, keyword)){
						id_array[i] = (uint32_t) photo_data_.id_photo;
						i++;
					}
					curr_node = get_next_node(curr_node);

				}
				
				i = htonl(i);
				send(client_socket, &i, sizeof(i), 0);
				i = ntohl(i);
				
				for(j=0; j != i; j++) {
					id_array[j] = htonl(id_array[j]);
					send(client_socket, &id_array[j], sizeof(id_array[j]), 0);
				}
			}
 
			/************* DEL PHOTO ****************/
			if(ntohl(type) == DEL_PHOTO) {
				recv(client_socket, &id_photo, sizeof(id_photo), 0);
				photo_data_.cli_sock = htonl(client_socket);
				photo_data_.peer_pid = htonl(getpid());
				photo_data_.id_photo = id_photo;

				send(gw_socket, &type, sizeof(type), 0);
				send(gw_socket, &photo_data_, sizeof(photo_data_), 0);


			}

			/********** GET PHOTO NAME **************/
			if(ntohl(type) == GET_NAME) {
				printf(KBLU"[Thread client]"RESET" Request to get photo name\n");
				res = recv(client_socket, &id_photo, sizeof(id_photo), 0);
				id_photo = ntohl(id_photo);
				curr_node = get_head(photo_data_list);
				while(curr_node != NULL) {
					photo_data_ = *(photo_data *)get_node_item(curr_node);
					if(photo_data_.id_photo == id_photo) {
						send(client_socket, photo_data_.file_name, sizeof(photo_data_.file_name), 0);
						printf(KBLU"[Thread client]"RESET" Sending photo name: %s\n", photo_data_.file_name);
						break;
					}
					curr_node = get_next_node(curr_node);
				}
				if(curr_node == NULL) {
					snprintf(photo_data_.file_name, sizeof(photo_data_.file_name), "%s", "\0");
					send(client_socket, photo_data_.file_name, sizeof(photo_data_.file_name), 0);
				}
			}

			/************* GET PHOTO ****************/
			if(ntohl(type) == GET_PHOTO) {
				printf(KBLU"[Thread client]"RESET" Request to get photo\n");
				recv(client_socket, &id_photo, sizeof(int), 0);
				id_photo = ntohl(id_photo);
				curr_node = get_head(photo_data_list);
				while(curr_node != NULL) {
					photo_data_ = *(photo_data *)get_node_item(curr_node);
					if(photo_data_.id_photo == id_photo) {
						
						photo_size = htonl(photo_data_.photo_size);
						send(client_socket, &photo_size, sizeof(photo_size), 0);
						
						printf(KBLU"[Thread client]"RESET" Sending photo: %s with size %d\n", photo_data_.file_name, photo_data_.photo_size);
						buffer = malloc(photo_data_.photo_size);
						sprintf(photo_name, "photos/id%d", photo_data_.id_photo);
						photo = fopen(photo_name, "rb");
                    	fread(buffer, 1, photo_data_.photo_size, photo);
                    	send(client_socket, buffer, photo_data_.photo_size, 0);
                    	fclose(photo);
						free(buffer);

						break;
					}
					curr_node = get_next_node(curr_node);
				}
				if(curr_node == NULL) {
					photo_size = htonl(0);
					send(client_socket, &photo_size, sizeof(photo_size), 0);
				}
			}
		}
		else {break;}
	}
	close(client_socket);
	pthread_exit(arg);
}

// Creates a new thread for each client
// Resolves client requests
// Must have syncronization between clients in photos
void *handle_clients(void * arg) {

	handle_client_arg *thread_arg = (handle_client_arg *)arg;
	int error;
	pthread_t thr_client;
	int sock_stream = (*thread_arg).client_sock;
	int ret;

	listen(sock_stream, 20);
	printf(KBLU"[Thread clients]"RESET" In clients thread\n");

	while(1) {
        
        printf(KBLU"[Thread clients]"RESET" Waiting connection from client...\n");

        (*thread_arg).client_sock = accept(sock_stream, NULL, NULL);
        
        error = pthread_create(&thr_client, NULL, handle_client, (void *)thread_arg);

		if(error != 0) {
			perror(KBLU"[Thread clients]"RESET" Unable to create thread to handle clients.");
			exit(-1);
		}
        pthread_detach(thr_client);
	}
}