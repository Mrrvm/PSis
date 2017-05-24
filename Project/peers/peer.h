#include "../defs.h"
#include "../generic_list.h"

void *handle_clients(void * arg);
void handle_rep(int socket, list* photo_data_list);
void *handle_ping_peer(void * arg);

typedef struct stream_sockets_{
	int client_sock;
	int gw_sock;
} stream_sockets;
