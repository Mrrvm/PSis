#include "structures.h"

void print_server(item got_item) {
    message_gw *message_gw_ = (message_gw *)got_item;
    spam(("Address %s:", message_gw_->address));
    spam(("%d\n", message_gw_->port));
    spam(("Type %d\n", message_gw_->type));
}

int get_server_port(item got_item) {
    message_gw *message_gw_ = (message_gw *)got_item;
    return message_gw_->port;
}

int main(){
    
    struct sockaddr_in local_addr;
    struct sockaddr_in temp_addr;
    socklen_t size_addr;
    int sock_local;
    message_gw *message_gw_, *aux_message;
    list *servers_list = create_list(sizeof(message_gw));

    // generates itself
    sock_local = socket(AF_INET, SOCK_DGRAM, 0);
    local_addr.sin_family = AF_INET;
    local_addr.sin_port= htons(3000);
    local_addr.sin_addr.s_addr= INADDR_ANY;
    bind(sock_local, (struct sockaddr *)&local_addr, sizeof(local_addr));

    // define message_gw
    message_gw_ = malloc(sizeof(message_gw));
    aux_message = malloc(sizeof(message_gw));

    while(1) {
        // waits contact from anyone
        size_addr = sizeof(temp_addr);
        recvfrom(sock_local, message_gw_, sizeof(*message_gw_), 0, 
              (struct sockaddr *) &temp_addr, &size_addr);

        if(message_gw_->type == SER_GW) {
            printf("Received server %s:%d\n\n", message_gw_->address,
                 message_gw_->port);
            push_item_to_list(servers_list, message_gw_);
            print_list(servers_list, print_server);

            // stores this contact in a list - NOT DONE YET
            aux_message->type = SER_AVB;
            strcpy(aux_message->address, message_gw_->address);
            aux_message->port = message_gw_->port;
            
        }
        else if(message_gw_->type == CLI_GW) {        
            // sends a response to the client
            printf("Client is requesting\n");
            sendto(sock_local, aux_message, sizeof(*aux_message), 0,
                (const struct sockaddr *) &temp_addr, 
                sizeof(temp_addr));
        }
    }
}

