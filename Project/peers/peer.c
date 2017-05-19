#include "peer.h"

int main(int argc, char *argv[])
{
    struct sockaddr_in local_addr;
    struct sockaddr_in gateway_addr;
    struct sockaddr_in gateway_addr_st;
    int sock_gw = 0;
    int sock_stream_client = 0;
    int sock_stream_gw = 0;
    int one_more_socket = 1;
    int local_port = 3000+getpid();
    pthread_t thr_clients;
    int error;
    void *ret;

    if(argc != 2) {
        printf("Invalid execution. Please use:\n./program [hostname]\n");
        exit(1);
    }

    printf("Created server: %d\n", local_port);

    // Set datagram socket with gateway
    sock_gw = socket(AF_INET, SOCK_DGRAM, 0);
    gateway_addr.sin_family = AF_INET;
    gateway_addr.sin_port = htons(3001);
    inet_aton(argv[1], &gateway_addr.sin_addr);

    // Set stream socket for clients
    sock_stream_client = socket(AF_INET, SOCK_STREAM, 0);
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(local_port);
    inet_aton(argv[1], &local_addr.sin_addr);
    bind(sock_stream_client, (struct sockaddr *)&local_addr, sizeof(local_addr));

    sock_stream_gw = socket(AF_INET, SOCK_STREAM, 0);
    gateway_addr_st.sin_family = AF_INET;
    gateway_addr_st.sin_port = htons(3002);
    inet_aton(argv[1], &gateway_addr_st.sin_addr);
    setsockopt(sock_stream_gw, SOL_SOCKET, SO_REUSEADDR, &one_more_socket, sizeof(int));
    
    // Send info to gateway
    if(-1 != sendto(sock_gw,(const struct sockaddr *) &local_addr, sizeof(local_addr), 0,
        (const struct sockaddr *) &gateway_addr, 
        sizeof(gateway_addr))) {

        // Set stream socket for sending info to gateway
        printf("Connecting stream socket to the gateway\n");
        connect(sock_stream_gw, (const struct sockaddr *) &gateway_addr_st, sizeof(gateway_addr_st));
        printf("sock: %d, errno: %d\n", sock_stream_gw, errno);
        
        // Creates a structure with stream sockets to send as a thread argument
        stream_sockets *ssockets = NULL;
        ssockets = malloc(sizeof(stream_sockets));
        (*ssockets).gw_sock = sock_stream_gw;
        (*ssockets).client_sock = sock_stream_client;

        // Thread 1: Pings the gateway

        // Thread 2: Waits for clients
        error = pthread_create(&thr_clients, NULL, handle_clients, (void *)ssockets);
        if(error != 0) {
            perror("Unable to create thread to handle clients.");
            exit(-1);
        }

        while(1) {
            handle_rep(sock_stream_gw);
        }

        pthread_join(thr_clients, (void*)&ret);
        
    }

    exit(0);
}