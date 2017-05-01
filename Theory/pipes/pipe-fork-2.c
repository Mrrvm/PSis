#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> 

int main(){
	int p_fd[2];
	int array[2];
	int valor = 12;
	int v_read;
	

//	if(pipe2(p_fd, O_NONBLOCK )==-1){
	if(pipe(p_fd)==-1){
		exit(-1);
	}
	printf("pipe was created\n");

	if(fork()==0){
		int k;
		close(p_fd[1]);
		//child process
		while((k = read(p_fd[0],&array, 2*sizeof(int)))>0){
			printf("read %d bytes\n", k);
				printf("%d %d\n", getpid(), array[0]);
				printf("%d %d\n", getpid(), array[1]);							
		}
	}else{
		char line[100];
		close(p_fd[0]);
		int values[3];
		for(int i= 0; i <6; i++){
			printf("Write something, please: ");
			scanf("%d", &values[0]);
			scanf("%d", &values[1]);
			scanf("%d", &values[2]);

			write(p_fd[1],&values, 3*sizeof(int)); 
		}
		close(p_fd[1]);
	}
	int status;
	wait(&status);
	exit(0);
}
