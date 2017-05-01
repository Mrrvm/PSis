#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> 

int main(){
	int p_fd[2];
	int valor = 12;
	int v_read;

//	if(pipe2(p_fd, O_NONBLOCK )==-1){
	if(pipe2(p_fd, 0 )==-1){

		exit(-1);
	}
	printf("pipe was created\n");

	if(read(p_fd[0],&v_read, sizeof(int))>0){
		printf("%d\n", v_read);
	} 	
	write(p_fd[1],&valor, sizeof(int)); 

	
}
