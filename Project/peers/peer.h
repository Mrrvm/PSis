#include "../defs.h"
#include "../structures.h"

void *handle_clients(void * arg);
void *handle_client(void * arg);

typedef struct conn_args_ {
	int sock_client_;
	struct sockaddr_in client_addr_;
} conn_args;