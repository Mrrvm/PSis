#include "peer.h"

int main(int argc, char *argv[])
{
	struct sockaddr_in local_addr;
	struct sockaddr_in gateway_addr;
	int sock_gw;
	int sock_stream;
	int local_port = 3000+getpid();
	pthread_t thr_clients;
    int error;
	void *ret;

	if(argc != 2) {
		printf("Invalid execution. Please use:\n./program [hostname]\n");
		exit(1);
	}

	printf("Created server: %d\n", local_port);

	// Gateway settings
    sock_gw = socket(AF_INET, SOCK_DGRAM, 0);
    gateway_addr.sin_family = AF_INET;
    gateway_addr.sin_port = htons(3001);
    inet_aton(argv[1], &gateway_addr.sin_addr);

    // Stream socket creation
    sock_stream = socket(AF_INET, SOCK_STREAM, 0);
    local_addr.sin_family = AF_INET;
    local_addr.sin_port= htons(local_port);
    inet_aton(argv[1], &local_addr.sin_addr);
    bind(sock_stream, (struct sockaddr *)&local_addr, sizeof(local_addr));

    // Send info to gateway
    sendto(sock_gw,(const struct sockaddr *) &local_addr, sizeof(local_addr), 0,
        (const struct sockaddr *) &gateway_addr, 
        sizeof(gateway_addr));

	// Thread 1: Pings the gateway


	exit(0);
}

