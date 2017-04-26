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

#define SOCKET_NAME "xxx"
#define MESSAGE_LEN 100

typedef struct message{
    char buffer[MESSAGE_LEN];
} message;


int main(){
    
    int sock_gate;
    struct sockaddr_in addr;
    message m;
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3000);  
    addr.sin_addr.s_addr = INADDR_ANY;

    /* create socket gateway */ 
    sock_gate = socket(AF_INET, SOCK_DGRAM, 0);
    bind(sock_gate, (struct sockaddr *)  &addr, sizeof(addr)); 
        
    while(1){
        recv(sock_gate, m.buffer, 100, 0);
        printf("%s\n", m.buffer);        
    }

    exit(0);
    
}

