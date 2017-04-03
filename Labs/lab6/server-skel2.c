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
    int addr_len;
  
	story = strdup("");
	
    unlink(SOCKET_NAME);
    /* create socket  */ 
    sock_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_NAME);

    bind(sock_fd, (struct sockaddr *)  &addr, sizeof(addr));
        
    while(1){
        /* read message */
        recvfrom(sock_fd, m.buffer, MESSAGE_LEN, 0, (struct sockaddr *)&client_addr, &addr_len);
        printf("message received: %s\n", m.buffer);
        /* process message */
        story = strcat(story, m.buffer);
        sendto(sock_fd, story, strlen(story) +1, 0, ( struct sockaddr *) &client_addr, sizeof(client_addr));
        
    }
    printf("OK\n");
    exit(0);
    
}
