#include "peer.h"

int main(int argc, char *argv[])
{
    struct sockaddr_in local_addr;
    struct sockaddr_in gateway_addr;
    struct sockaddr_in gateway_addr_st;
    int sock_gw_ping = 0, sock_gw = 0, sock_stream_client = 0, sock_stream_gw = 0;
    int reuse_socket = 1, error = 0, res = 0, i = 0, n_nodes = 0, photo_size = 0;
    int local_port = 3000+getpid();
    int n;
    pthread_t thr_clients;
    pthread_t thr_ping_peer;
    void *ret;
    char photo_name[100], *buffer;
    FILE *photo;
    photo_data photo_data_;
    list *photo_data_list = create_list(sizeof(photo_data));

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
    setsockopt(sock_stream_client, SOL_SOCKET, SO_REUSEADDR, &reuse_socket, sizeof(int));
    bind(sock_stream_client, (struct sockaddr *)&local_addr, sizeof(local_addr));

    // Set stream socket for gateway
    sock_stream_gw = socket(AF_INET, SOCK_STREAM, 0);
    gateway_addr_st.sin_family = AF_INET;
    gateway_addr_st.sin_port = htons(3002);
    inet_aton(argv[1], &gateway_addr_st.sin_addr);
    setsockopt(sock_stream_gw, SOL_SOCKET, SO_REUSEADDR, &reuse_socket, sizeof(int));

    // Set socket for the ping comunication
    sock_gw_ping = socket(AF_INET, SOCK_DGRAM, 0);
    bind(sock_gw_ping, (struct sockaddr *)&local_addr, sizeof(local_addr));

    // Send info to gateway
    if(-1 != sendto(sock_gw ,(const struct sockaddr *) &local_addr, sizeof(local_addr), 0,
        (const struct sockaddr *) &gateway_addr, 
        sizeof(gateway_addr))) {

        // Set stream socket for sending info to gateway
        printf("Connecting stream socket to the gateway\n");
        connect(sock_stream_gw, (const struct sockaddr *) &gateway_addr_st, sizeof(gateway_addr_st));
        
        // Creates a structure with stream sockets to send as a thread argument
        handle_client_arg *thread_arg = NULL;
        thread_arg = malloc(sizeof(handle_client_arg));
        (*thread_arg).gw_sock = sock_stream_gw;
        (*thread_arg).client_sock = sock_stream_client;
        (*thread_arg).photo_data_list = photo_data_list;

        // Retrieves previous data on the system
        res = recv(sock_stream_gw, &n_nodes, sizeof(n_nodes), 0);
        if(sizeof(n_nodes) == res) {
            n_nodes = ntohl(n_nodes);
            printf("Ready to receive %d previous list nodes\n", n_nodes);
            while(i != n_nodes) {
                res = recv(sock_stream_gw, &photo_data_, sizeof(photo_data_), 0);
                if(sizeof(photo_data_) == res) {
                    photo_data_.id_photo = ntohl(photo_data_.id_photo);
                    photo_size = photo_data_.photo_size = ntohl(photo_data_.photo_size);
                    push_item_to_list(photo_data_list, &photo_data_);
                    print_list(photo_data_list, print_photo);
                    buffer = malloc(photo_size);
                    if(buffer == NULL) {
                        printf("Unable to alloc buffer\n");
                        exit(-1);
                    }

                    n=0;
                    while(n != photo_size){
                        res = recv(sock_stream_gw, buffer+n, photo_size-n, 0);
                        n += res;
                    }
                    if(photo_size == n) {
                        printf("Received photo of size %d!\n", photo_data_.photo_size);
                        sprintf(photo_name, "photos/id%d", photo_data_.id_photo);
                        photo = fopen(photo_name, "wb");
                        fwrite(buffer, 1, photo_data_.photo_size, photo);
                        fclose(photo);
                    } 
                    else {
                        printf("Unable to retrieve previous data.\n");
                        exit(1);
                    }
                    free(buffer); 
                    i++;
                }
                else {
                    printf("Unable to retrieve previous data.\n");
                    exit(1);
                }
            }
            i = 0;
        }
        
        // Thread 1: Pings the gateway
        error = pthread_create(&thr_ping_peer, NULL, handle_ping_peer, &sock_gw_ping);
        if(error != 0) {
            perror("Unable to create thread to handle pings.");
            exit(-1);
        }

        // Thread 2: Waits for clients
        error = pthread_create(&thr_clients, NULL, handle_clients, (void *)thread_arg);
        if(error != 0) {
            perror("Unable to create thread to handle clients.");
            exit(-1);
        }

        // Thread 3: Handles replication 
        handle_rep(sock_stream_gw, photo_data_list);

        pthread_join(thr_clients, (void*)&ret); 
        pthread_join(thr_ping_peer, (void*)&ret);
    }

    exit(0);
}