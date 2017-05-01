#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> 

int main(){
	int p_fd[2], pp_fd[2];
	int valor = 12;
	int v_read;

//	if(pipe2(p_fd, O_NONBLOCK )==-1){
	if(pipe(p_fd)==-1){
		exit(-1);
	}
	if(pipe(pp_fd)==-1){
		exit(-1);
	}
	printf("pipe was created\n");

	if(fork()==0){
		int k;
		close(p_fd[1]);
		//child process
		while(read(p_fd[0],&v_read, sizeof(int))>0){
				printf("%d %d\n", getpid(), v_read);
				k = v_read * 10;
				write( pp_fd[1]    , &k, sizeof(int));
				       
							
		}
	}else{
		char line[100];
		close(p_fd[0]);
		for(int i= 0; i <5; i++){
			printf("write something, please: ");
			scanf("%d", &valor);
			write(p_fd[1],&valor, sizeof(int)); 
			read(pp_fd[0],&valor, sizeof(int)); 
			printf("XXXX %d %d\n", getpid(), valor);
			
		}
		close(p_fd[1]);
	}
	int status;
	wait(&status);
	exit(0);
}
