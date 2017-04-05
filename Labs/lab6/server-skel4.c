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
    
    message m;
    int sock_fd, new_sock;
    char *story;

    struct sockaddr_in addr;

    unlink(SOCKET_NAME);
    /* create socket  */ 
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;

    addr.sin_port = htons(3000);        /* nmero de porto */
    //inet_aton("146.193.41.15", & addr.sin_addr);  /* endereo IP */
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(sock_fd, (struct sockaddr *)  &addr, sizeof(addr));
    listen(sock_fd, 10);  

    while(1){
        new_sock = accept(sock_fd, NULL, NULL);
        printf("Accepted!\n");
        recv(new_sock, m.buffer, 100, 0);
        printf("%s\n", m.buffer);
        close(new_sock);        
    }
    exit(0);
}
