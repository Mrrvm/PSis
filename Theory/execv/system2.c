#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
	int pid = getpid();
	int ret;
	printf("I'm execv1 %d\n", pid);
	getchar();
	int ret_pid = fork();
	if(ret_pid == 0){
		int ret =	execl("./prog1", "./prog1", NULL);
	}else{
		int child_status;
		int killed_child = wait(&child_status);
		if( WIFEXITED(child_status)){
			printf("process %d terminaed normally\n", 
				killed_child);
			printf("return value %d\n", WEXITSTATUS(child_status));
		}else{
			printf("process %d was killed", 
				killed_child);

		}
		exit(0);
	}	


}