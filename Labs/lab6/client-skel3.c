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
 
    int sock_fd;  
    message m;  
    struct sockaddr_un addr;

    /* create socket  */ 
    sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_NAME);
    
    connect(sock_fd, ( struct sockaddr *) &addr, sizeof(addr));
    printf("Message:\n");
    fgets(m.buffer, MESSAGE_LEN, stdin);
    send(sock_fd, m.buffer, strlen(m.buffer)+1, 0);
    exit(0);
    
}
