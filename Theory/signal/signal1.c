#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int i = 0;

void reset_i(int n){
	i = 0;
}

void terminate_ok(int n){
	int in;
	printf("received signal %d, do you want to exit? (0/1)", n);
	scanf("%d", &in);
	if(in==0){
		return;
	}else{
		exit(-1);
	}
}
int main(){
	int pid = getpid();
	int ret;
	printf("I'm execv1 %d\n", pid);
	int ret_pid = fork();
	if(ret_pid == 0){
		signal(31, SIG_IGN);
		signal(30, reset_i);
		signal(SIGINT, terminate_ok);

		while(1){
			sleep(1);
			i++;
			printf("%d\n", i);
		}
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
			if(WIFSIGNALED(child_status))
				printf(" Signal %d\n", WTERMSIG(child_status));

		}
		exit(0);
	}	


}