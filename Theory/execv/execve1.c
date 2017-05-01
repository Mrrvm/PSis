#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
	int pid = getpid();
	printf("I'm execv1 %d\n", pid);
	getchar();
	int ret =	execl("./prog11", "./prog1", NULL);
	printf("returned value %d", ret);
	exit(0);



}