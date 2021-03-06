#include "../generic_list.h"

void *handle_cli_requests(void * arg);
void *handle_peers(void * arg);
void handle_ping_gw(list *servers_list);


typedef struct handle_peer_arg_ {
	list *servers_list;
	int peer_socket;
	int id_counter;
} handle_peer_arg;