#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

int main(int argc, char const *argv[])
{
	
	int i;
	int pid;
	int sleeptime;

	
	for(i = 0; i < 10; ++i)
	{	

		pid = fork();

		if(pid == 0)
		{	
			srandom(time(NULL)^i);

			sleeptime = random()%10;
			sleep(sleeptime);

			printf("\n[Child %d] My PID is %d\n", i, getpid() );
			printf("I slept for %d seconds\n", sleeptime );

			return 0;


		}
	}




	return 0;
}