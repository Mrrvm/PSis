#include "defs.h"

char *touppercase(char *str) {
  // Convert to upper case
  char *s = str;
  while (*s) {
    *s = toupper((unsigned char) *s);
    s++;
  }
  return s;
}

int main(){
    
    struct sockaddr_in local_addr;
    struct sockaddr_in gateway_addr;
    struct sockaddr_in client_addr;
    socklen_t size_addr;
    int sock_gw;
    int sock_stream;
    int sock_client;
    message_gw *message_gw_;
    message_std *message_std_;
    int local_port = 3001;
    char buff[100];

    // gateway socket
    sock_gw = socket(AF_INET, SOCK_DGRAM, 0);
    gateway_addr.sin_family = AF_INET;
    gateway_addr.sin_port = htons(3000);
    inet_aton(MY_IP, &gateway_addr.sin_addr);

    // client socket
    sock_stream = socket(AF_INET, SOCK_STREAM, 0);
    local_addr.sin_family = AF_INET;
    local_addr.sin_port= htons(local_port);
    inet_aton(MY_IP, &local_addr.sin_addr);
    bind(sock_stream, (struct sockaddr *)&local_addr, sizeof(local_addr));

    // sends address to gateway
    message_gw_ = malloc(sizeof(message_gw));
    message_gw_->type = SER_GW;
    strcpy(message_gw_->address, MY_IP);
    message_gw_->port = local_port;
    sendto(sock_gw, message_gw_, sizeof(*message_gw_), 0,
                (const struct sockaddr *) &gateway_addr, 
                sizeof(gateway_addr));

    listen(sock_stream, 1); // this allows only 1 client to connect
    message_std_ = malloc(sizeof(message_std));

    while(1) {
        sock_client = accept(sock_stream, 
                    (struct sockaddr *) & client_addr, &size_addr);
        printf("Waiting a client to connect...\n");
        // waits messages from client
        recv(sock_client, message_std_, sizeof(*message_std_), 0);
        // prints it 
        printf("Message is %s\n", message_std_->buffer);

        // sends message back to client in UPPER_CASE  
        close(sock_client);
    }
    close(sock_stream);
}

