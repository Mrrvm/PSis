#include "../defs.h"
#include "../generic_list.h"

void *handle_clients(void * arg);
void handle_rep(int socket, list* photo_data_list);
void *handle_ping_peer(void * arg);

typedef struct handle_client_arg_{
	int client_sock;
	int gw_sock;
	list *photo_data_list;
} handle_client_arg;
