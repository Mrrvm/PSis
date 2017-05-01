#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h> 
#include <unistd.h>
#include <arpa/inet.h>



int main(){
	struct sockaddr_in local_addr;
	struct sockaddr_in client_addr;
	socklen_t size_addr;
	char buff[100];
	int nbytes;
	
	int sock_fd= socket(AF_INET, SOCK_DGRAM, 0);
	
	if (sock_fd == -1){
		perror("socket: ");
		exit(-1);
	}
	

	local_addr.sin_family = AF_INET;
	//	local_addr.sin_port= htons(3001);
		local_addr.sin_port= (3001);
	local_addr.sin_addr.s_addr= INADDR_ANY;
	
	int err = bind(sock_fd, (struct sockaddr *)&local_addr, sizeof(local_addr));
	if(err == -1) {
		perror("bind");
		exit(-1);
	}
	
	printf(" socket created and binded \n Ready to receive messages\n");

	while(1){
	  size_addr = sizeof(client_addr);
	  nbytes = recvfrom(sock_fd, buff, 100, 0, 
	          (struct sockaddr *) & client_addr, &size_addr);
	printf("received %d bytes from %s %d --- %s ---\n", 
	       nbytes, inet_ntoa(client_addr.sin_addr),client_addr.sin_port,  buff);
	      
	 sprintf(buff, "reply to %s", buff);
	nbytes = sendto(sock_fd, 
	                    buff, strlen(buff)+1, 0, 
	                    (const struct sockaddr *) &client_addr, 
	                    sizeof(client_addr));
	 printf("replying %d bytes\n", nbytes);
      }	                    

	exit(0);
}

