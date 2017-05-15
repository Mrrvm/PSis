#include "../defs.h"
#include "../structures.h"

void *handle_clients(void * arg);

struct conn_args_ {
	int sock_stream_;
	struct sockaddr_in client_addr_;
} conn_args;