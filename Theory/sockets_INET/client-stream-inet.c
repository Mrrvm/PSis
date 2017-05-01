#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h> 
#include <unistd.h>
#include <arpa/inet.h>

//193.136.128.104

int main(){
	struct sockaddr_in server_addr;

	char buff[100];
	int nbytes;
	
	int sock_fd= socket(AF_INET, SOCK_STREAM, 0);
	
	if (sock_fd == -1){
		perror("socket: ");
		exit(-1);
	}



	printf(" socket created \n");

	printf(" socket with adress \n");


	server_addr.sin_family = AF_INET;
	// this values can be read from the keyboard
	server_addr.sin_port= htons(3000);
	inet_aton("193.136.128.104", &server_addr.sin_addr);
	 
	
	if( -1 == connect(sock_fd, 
			(const struct sockaddr *) &server_addr, 
			sizeof(server_addr))){
				printf("Error connecting\n");
				exit(-1);
	}
	
	sprintf(buff, "first message");
	nbytes = send(sock_fd, 
	                    buff, strlen(buff)+1, 0);
	printf("sent %d %s\n", nbytes, buff);
	nbytes = recv(sock_fd, buff, 100, 0);
	printf("received %d bytes --- %s ---\n", nbytes, buff);        

sleep(1);

	sprintf(buff, "second message");
	nbytes = send(sock_fd, 
	                    buff, strlen(buff)+1, 0);
	printf("sent %d %s\n", nbytes, buff);

	nbytes = recv(sock_fd, buff, 100, 0);
	printf("received %d bytes --- %s ---\n", nbytes, buff);        
sleep(1);
	sprintf(buff, "third message");
	nbytes = send(sock_fd, 
	                    buff, strlen(buff)+1, 0);
	printf("sent %d %s\n", nbytes, buff);
	nbytes = recv(sock_fd, buff, 100, 0);
	printf("received %d bytes --- %s ---\n", nbytes, buff);        

	close(sock_fd);
	exit(0);
}
