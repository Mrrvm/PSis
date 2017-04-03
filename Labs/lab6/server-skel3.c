#include "storyserver.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

int main(){
    
    int sock_fd, new_sock;
    message m;
    struct sockaddr_un addr, client_addr;

    unlink(SOCKET_NAME);
    /* create socket  */ 
    sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_NAME);

    bind(sock_fd, (struct sockaddr *)  &addr, sizeof(addr));
    listen(sock_fd, 10);  

    while(1){

        new_sock = accept(sock_fd, NULL, NULL);
        printf("Accepted!\n");
        close(new_sock);
        
    }
    exit(0);
}
