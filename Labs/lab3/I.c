#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

int main(int argc, char const *argv[])
{
	int i = 0, ret = 0, time_sleeping = 0;
	srand(time(NULL));

	printf("In reproduction...\n");


	while(i < 10) {
		time_sleeping = rand()%10;
		ret = fork();
		if(ret == 0)
			break;
		i++;
	}
	if(ret == 0) {
		sleep(time_sleeping);
		printf("[Child %d]: I was born with the pid %d and slept", i, getpid());
		printf(" %d seconds\n", time_sleeping);
		exit(-1);
	}
	exit(0);
}

