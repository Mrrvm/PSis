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

#include <arpa/inet.h>
int main(){
 
    int sock_fd;  
    message m;  
    struct sockaddr_in addr;

    /* create socket  */ 
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;

    addr.sin_port = htons(3000); 
    //inet_aton("127.0.0.1", &server_addr.sin_addr);
    addr.sin_addr.s_addr = INADDR_ANY;
    
    connect(sock_fd, ( struct sockaddr *) &addr, sizeof(addr));
    printf("Message:\n");
    fgets(m.buffer, MESSAGE_LEN, stdin);
    send(sock_fd, m.buffer, strlen(m.buffer)+1, 0);
    exit(0);
    
}
