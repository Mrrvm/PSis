#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

int main(int argc, char const *argv[])
{
	int i = 0, ret = 0;

	printf("In reproduction...\n");

	while(i <= 10) {
		ret = fork();
		if(ret == 0) {
			sleep(random()%10);
			printf("[Child %d]: I was born with the pid %d \n", i, getpid());
			return 0;
		}
		i++;
	}
	exit(0);
}