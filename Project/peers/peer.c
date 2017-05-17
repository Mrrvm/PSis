#include "peer.h"

int main(int argc, char *argv[])
{
	struct sockaddr_in local_addr;
	struct sockaddr_in gateway_addr;
	int sock_gw;
	int sock_stream;
	int local_port = 3000+getpid();
	pthread_t thr_clients;
    int error;
	void *ret;
	peer_data* peer_data_;

	if(argc != 2) {
		printf("Invalid execution. Please use:\n./program [hostname]\n");
		exit(1);
	}

	printf("Created server: %d\n", local_port);

	// Gateway settings
    sock_gw = socket(AF_INET, SOCK_DGRAM, 0);
    gateway_addr.sin_family = AF_INET;
    gateway_addr.sin_port = htons(3001);
    inet_aton(argv[1], &gateway_addr.sin_addr);

    // Stream socket creation
    sock_stream = socket(AF_INET, SOCK_STREAM, 0);
    local_addr.sin_family = AF_INET;
    local_addr.sin_port= htons(local_port);
    inet_aton(argv[1], &local_addr.sin_addr);
    bind(sock_stream, (struct sockaddr *)&local_addr, sizeof(local_addr));

    peer_data_ = malloc(sizeof(peer_data));

    peer_data_->port = local_addr.sin_port;
    strcpy(argv[1], peer_data_->address);
    peer_data_->sock_peer = htonl(0);


    // Send info to gateway
    if(-1 != sendto(sock_gw, peer_data_, sizeof(*peer_data_), 0,
        (const struct sockaddr *) &gateway_addr, 
        sizeof(gateway_addr))) {

		// Thread 1: Pings the gateway

		// Thread 2: Waits for clients
		error = pthread_create(&thr_clients, NULL, handle_clients, &sock_stream);
		if(error != 0) {
			perror("Unable to create thread to handle clients.");
			exit(-1);
		}

		pthread_join(thr_clients, (void*)&ret);
    	
    }

	exit(0);
}

