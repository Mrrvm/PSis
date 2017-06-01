#include "peer.h"

void *handle_client(void * arg) {

	handle_client_arg *thread_arg = (handle_client_arg *)arg;
	int client_socket = (*thread_arg).client_sock;
	int gw_socket = (*thread_arg).gw_sock;
	list *photo_data_list = (*thread_arg).photo_data_list;
	photo_data photo_data_;
	int photo_size = 0;
	char *buffer;
	int res;
	int type;
	int ret;
	int id_photo;
	node *curr_node = NULL;
	char photo_name[100];
	FILE *photo;

	printf("Handling client\n");

	while(1) {

		res = recv(client_socket, &type, sizeof(int), 0);
		if(sizeof(int) >= res && res > 0) {
			
			/************* ADD PHOTO ****************/
			if(ntohl(type) == ADD_PHOTO) {
				send(gw_socket, &type, sizeof(int), 0);
				res = recv(client_socket, &photo_data_, sizeof(photo_data_), 0);
				if(sizeof(photo_data_) >= res && res > 0) {
					send(gw_socket, &photo_data_, sizeof(photo_data_), 0);
					// Checks which type of request it is:	
					photo_size = ntohl(photo_data_.photo_size);			
					buffer = malloc(photo_size);
					if(buffer == NULL){
						printf("Unable to alloc buffer\n");
						exit(0);
					}
					res = recv(client_socket, buffer, photo_size, 0);
					if(photo_size >= res && res > 0) {
						printf("Received photo of size %d\n", photo_size);
						send(gw_socket, buffer, photo_size, 0);
						free(buffer);
						usleep(4800); // Gives time for the photo to be replicated
						// ^READ LOCK?
						curr_node = get_head(photo_data_list);
						if(curr_node != NULL) {
							photo_data_ = *(photo_data *)get_node_item(curr_node);
							ret = photo_data_.id_photo; 
							if(ret >= 0) {
								send(client_socket, &ret, sizeof(int), 0);
								continue;
							}
						}
					}
				}				
				ret = 0;
				send(gw_socket, &ret, sizeof(int), 0);
				break;
			}
 
			/************* DEL PHOTO ****************/
			if(ntohl(type) == DEL_PHOTO) {
				recv(client_socket, &id_photo, sizeof(id_photo), 0);
				send(gw_socket, &type, sizeof(type), 0);
				send(gw_socket, &id_photo, sizeof(id_photo), 0);


			}

			/************* GET PHOTO ****************/
			if(ntohl(type) == GET_PHOTO) {
				printf("Request to get photo\n");
				recv(client_socket, &id_photo, sizeof(int), 0);
				curr_node = get_head(photo_data_list);
				while(curr_node != NULL) {
					photo_data_ = *(photo_data *)get_node_item(curr_node);
					if(photo_data_.id_photo == id_photo) {
						
						photo_size = htonl(photo_data_.photo_size);
						send(client_socket, &photo_size, sizeof(photo_size), 0);
						
						printf("Sending photo: %s with size %d\n", photo_data_.file_name, photo_data_.photo_size);
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
	printf("In clients thread\n");

	while(1) {
        
        printf("Waiting connection from client...\n");

        (*thread_arg).client_sock = accept(sock_stream, NULL, NULL);
        
        error = pthread_create(&thr_client, NULL, handle_client, (void *)thread_arg);

		if(error != 0) {
			perror("Unable to create thread to handle clients.");
			exit(-1);
		}
        pthread_detach(thr_client);
	}
}