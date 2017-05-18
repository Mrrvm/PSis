#include "../defs.h"

void *handle_clients(void * arg);
void handle_replication(int sock);

typedef struct stream_sockets_{
	int client_sock;
	int gw_sock;
} stream_sockets;
