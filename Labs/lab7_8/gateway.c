#include "defs.h"

int main(){
    
    struct sockaddr_in local_addr;
    struct sockaddr_in temp_addr;
    socklen_t size_addr;
    int sock_local;
    message_gw *message_gw_;

    // generates itself
    sock_local = socket(AF_INET, SOCK_DGRAM, 0);
    local_addr.sin_family = AF_INET;
    local_addr.sin_port= htons(3000);
    local_addr.sin_addr.s_addr= INADDR_ANY;
    bind(sock_local, (struct sockaddr *)&local_addr, sizeof(local_addr));

    // define message_gw
    message_gw_ = malloc(sizeof(message_gw));

    while(1) {
        // waits contact from any server
        // stores this contact in a list - NOT DONE YET
        size_addr = sizeof(temp_addr);
        recvfrom(sock_local, message_gw_, sizeof(*message_gw_), 0, 
              (struct sockaddr *) &temp_addr, &size_addr);
        printf("%s\n", message_gw_->address);

        // waits requests from client
        // sends a response to the client
    }

}

