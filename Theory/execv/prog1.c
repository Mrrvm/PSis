#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
	int pid = getpid();

	printf("I'm program 1 %d \n", pid);
		getchar();

	exit(123);
}