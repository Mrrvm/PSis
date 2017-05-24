#include "defs.h"

void print_photo(item got_item) {
    photo_data *photo_data_ = (photo_data *)got_item; 
    printf("%d", photo_data_->id_photo);
}

void print_server(item got_item) {
    peer_data *peer_data_ = (peer_data *)got_item;
    struct sockaddr_in peer_addr = peer_data_->peer_addr; 
    printf("%d", ntohs(peer_addr.sin_port));
}