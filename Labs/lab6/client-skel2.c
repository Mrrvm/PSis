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
 
    message m;  
    struct sockaddr_in addr, client_addr;
    int sock_fd;
    int i = 0;
    char story[100];
    
    /* create socket  */ 
    sock_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_NAME);

    client_addr.sun_family = AF_UNIX;
    sprintf(client_addr.sun_path, "Socket_%d", getpid());
    bind(sock_fd, (struct sockaddr *)  &client_addr, sizeof(client_addr));

    while(1) {
	    printf("message: ");
	    fgets(m.buffer, MESSAGE_LEN, stdin);
	    /* write message */
	    sendto(sock_fd, m.buffer, strlen(m.buffer)+1, 0, ( struct sockaddr *) &addr, sizeof(addr));
        recv(sock_fd, story, 100, 0);
        printf("%s", story);
    }
    /* receive story */

    printf("OK\n");
    exit(0);
    
}
