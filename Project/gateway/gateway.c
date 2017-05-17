#include "gateway.h"

// port 3000 - client requests
// port 3001 - peer entries (list)

int main() {

	pthread_t thr_cli;
	pthread_t thr_peerlist;
	pthread_t thr_replication
	peer_data peer_data_;
	int error;
	void *ret;

	list *servers_list = create_list(sizeof(peer_data));

	// Thread 1: Resolves client requests in client socket
	// Sends an address back according to the round-robin approach
    error = pthread_create(&thr_cli, NULL,	handle_cli_requests, servers_list);
	if(error != 0) {
		perror("Unable to create thread to handle client requests.");
		exit(-1);
	}
	
	// Thread 2: Manages peer info in peer socket
	// Adds it to the peer list
	error = pthread_create(&thr_peerlist, NULL,	handle_peer_list, servers_list);
	if(error != 0) {
		perror("Unable to create thread to handle peer list.");
		exit(-1);
	}

	// Thread 3: Manages replication between peers
	error = pthread_create(&thr_replication, NULL,	handle_replication, servers_list);
	if(error != 0) {
		perror("Unable to create thread to handle peer list.");
		exit(-1);
	}

	pthread_join(thr_cli, (void*)&ret);
	pthread_join(thr_peerlist, (void*)&ret);
	pthread_join(thr_replication, (void*)&ret);

	exit(0);
}


