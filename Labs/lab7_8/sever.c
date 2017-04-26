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

#define MESSAGE_LEN 100
#define CLI_GW 0
#define SER_GW 1
#define CLI_SER 2

typedef struct message{
	int type;
    char buffer[MESSAGE_LEN];
} message;


int main(){
    
    int sock_stream, sock_gate;
    struct sockaddr_in gate_addr, client_addr;
    message m;
 	
    /* socket gateway */
 	gate_addr.sin_family = AF_INET;
    gate_addr.sin_port = htons(3000);  
    inet_aton("194.210.134.31", &gate_addr.sin_addr);
    sock_gate = socket(AF_INET, SOCK_DGRAM, 0);

    /* socket stream/client */ 
    // sock_stream = socket(AF_INET, SOCK_STREAM, 0);
    // client_addr.sin_family = AF_INET;
    // client_addr.sin_port = htons(3001+getpid()); 
    // client_addr.sin_addr.s_addr = INADDR_ANY;
    // bind(sock_stream, (struct sockaddr *)  &client_addr, sizeof(client_addr));
    // listen(sock_stream, 10); 

    strcpy(m.buffer, "ola");
    sendto(sock_gate, m.buffer, strlen(m.buffer)+1, 0, (struct sockaddr *) &gate_addr, sizeof(gate_addr));
    exit(0);    
}

