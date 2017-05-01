#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	printf("Parent %d\n",getpid());
	int a= 1;
	int ret_val = fork();
	while(a);
	if(ret_val ==0){
		printf("[Child] - cries!\n");
		printf("[Child] -  i am %d\n",getpid());
	}else{
		printf("[Parent] My newborn is %d\n",ret_val);
		printf("[parent] -  i am %d\n",getpid());
	}

	exit(0);


}