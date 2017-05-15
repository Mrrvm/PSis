#include "peer.h"

int main(int argc, char *argv[])
{
	struct sockaddr_in local_addr;
	struct sockaddr_in gateway_addr;
	int sock_gw;
	int sock_stream;
	int local_port = 4000; //3000+getpid();
	pthread_t thr_clients;
    int error;
	void *ret;

	if(argc != 2) {
		printf("Invalid execution. Please use:\n./program [hostname]\n");
		exit(1);
	}

    // Stream socket creation
    sock_stream = socket(AF_INET, SOCK_STREAM, 0);
    local_addr.sin_family = AF_INET;
    local_addr.sin_port= htons(local_port);
    inet_aton(argv[1], &local_addr.sin_addr);
    bind(sock_stream, (struct sockaddr *)&local_addr, sizeof(local_addr));


		// Thread 2: Waits for clients
		error = pthread_create(&thr_clients, NULL, handle_clients, &sock_stream);
		if(error != 0) {
			perror("Unable to create thread to handle clients.");
			exit(-1);
		}

		pthread_join(thr_clients, (void*)&ret);
    	
    //}
	exit(0);
}

