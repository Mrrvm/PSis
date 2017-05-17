#include "../defs.h"

void *handle_clients(void * arg);

typedef struct stream_sockets_{
	int client_sock;
	int gw_sock;
} stream_sockets;