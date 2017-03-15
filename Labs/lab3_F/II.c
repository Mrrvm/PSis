#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>

int main(int argc, char const *argv[])
{
	
	int i, pid;
	int total_time_slept = 0;
	int sleeptime = 0;
	int statval = 0;

	for(i = 0; i < 10; ++i)
	{	
		pid = fork();

		if(pid == 0)
		{	
			srandom(time(NULL)^i);
			printf("\n[Child %d] Fell asleep\n", getpid());

			sleeptime = random()%10;

			sleep(sleeptime);
			printf("Sleeptime: %d\n", sleeptime);
			
			printf("[Child %d] Woke up\n", getpid());

			exit(sleeptime);

		}
		else
		{
			pid = wait(&statval);
			sleeptime = WEXITSTATUS(statval);
			total_time_slept = total_time_slept + sleeptime;
			printf("\n[Parent] Child %d slept for %d seconds\n", pid, sleeptime);
		}
	}

	printf("[Parent] Total time slept: %d\n", total_time_slept );
	return 0;
}