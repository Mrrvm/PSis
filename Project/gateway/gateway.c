#include "gateway.h"

// port 3000 - client requests

int main() {

	pthread_t thr_cli;
	int error;
	void *ret;

	// Thread 1: Resolves client requests in client socket
	// Sends an address back according to the roud-robin approach
    error = pthread_create(&thr_cli, NULL,	handle_cli_requests, NULL);
	if(error != 0) {
		perror("Unable to create thread to handle client requests.");
		exit(-1);
	}

	pthread_join(thr_cli, (void*)&ret);

	exit(0);
}



// (???) Not sure if 1 or 2 threads, 1 or 2 sockets
// Thread 2: 
	// Manages peer pings in peer socket
		// If one is dead, remove from the list
	// Manages peer info in peer socket
		// Adds it to the peer list

// Must have syncronization when reading/writing the list of peers
