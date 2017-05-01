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

  int sock_fd= socket(AF_INET, SOCK_STREAM, 0);

  if (sock_fd == -1){
    perror("socket: ");
    exit(-1);
  }


  local_addr.sin_family = AF_INET;
  local_addr.sin_port= htons(3000);
  local_addr.sin_addr.s_addr= INADDR_ANY;
  int err = bind(sock_fd, (struct sockaddr *)&local_addr, sizeof(local_addr));
  if(err == -1) {
    perror("bind");
    exit(-1);
  }
  printf(" socket created and binded \n");

  listen(sock_fd, 5);

  printf("Ready to accept connections\n");
  while(1){
    int client_fd= accept(sock_fd, (struct sockaddr *) & client_addr, &size_addr);

    if(fork()==0){
      printf("Accepted one connection from %s \n", inet_ntoa(client_addr.sin_addr));
      nbytes = recv(client_fd, buff, 100, 0);
      printf("received %d bytes --- %s ---\n", nbytes, buff);

      sprintf(buff, "reply 1");
      nbytes = send(client_fd, buff, strlen(buff)+1, 0);
      printf("replying %d bytes\n", nbytes);

      //sleep(1);
      nbytes = recv(client_fd, buff, 100, 0);
      printf("received %d bytes --- %s ---\n", nbytes, buff);
      sprintf(buff, "reply 2 ");
      nbytes = send(client_fd, buff, strlen(buff)+1, 0);
      printf("replying %d bytes\n", nbytes);

      //sleep(1);
      nbytes = recv(client_fd, buff, 100, 0);
      printf("received %d bytes --- %s ---\n", nbytes, buff);
      sprintf(buff, "reply 3 ");
      nbytes = send(client_fd, buff, strlen(buff)+1, 0);
      printf("replying %d bytes\n", nbytes);

      close(client_fd);
      printf("closeing connectin with client\n");
      exit(0);
    }
  }
  close(sock_fd);
  exit(0);
}
