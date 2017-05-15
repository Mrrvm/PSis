#include "peer.h"

// Creates a new thread for each client
// Resolves client requests
// Must have syncronization between clients in photos
void *handle_clients(void * arg) {

	int sock_stream = (int) arg;
	int sock_client;
	socklen_t size_addr;
	conn_args conn_args_;
	void *ret;
	int error;

	listen(sock_stream, 1);

	while(1) {
        
        size_addr = sizeof(conn_args_.client_addr);
        sock_client = accept(sock_stream, 
                    (struct sockaddr *) &conn_args_.client_addr, &size_addr);
	
        //alloc dyamic quantity of threads!
        // each thread resolves client requests with sync
	}
}