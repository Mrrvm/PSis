#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
	
	int i;
	int pid;
	int sleeptime;

	srandom(time(NULL));
	
	for(i = 0; i < 10; ++i)
	{	
		sleeptime = random()%10;
		pid = fork();

		if(pid==0)
			break;
	}

	if(pid == 0)
	{	
		sleep(sleeptime);

		printf("\n[Child %d] My PID is %d\n", i, getpid() );
		printf("I slept for %d seconds\n", sleeptime );

		return 0;
	}

	for (int i = 0; i < 10; ++i)
	{
		wait(NULL);
	}

	return 0;
}