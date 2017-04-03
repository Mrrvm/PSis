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
    char * story;
    struct sockaddr_un addr, client_addr;
  
	story = strdup("");
	
    /* create socket  */ 
    unlink(SOCKET_NAME);
    sock_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    addr.sun_family = AF_UNIX;

    strcpy(addr.sun_path, SOCKET_NAME);
    bind(sock_fd, (struct sockaddr *)  &addr, sizeof(addr));
        
    while(1){
        /* read message */
        recv(sock_fd, m.buffer, MESSAGE_LEN, 0);
        printf("message received: %s\n", m.buffer);
        /* process message */
        story = strcat(story, m.buffer);
        
    }
    printf("OK\n");
    exit(0);
    
}
