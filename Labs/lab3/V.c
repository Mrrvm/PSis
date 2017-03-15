#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

int alarm_flag = 0;

void alarm_flagger(int sig) {
	alarm_flag = 1;
}

int main(int argc, char const *argv[])
{
	int i = 0; 
	int alarm_time = 0;
	struct sigaction sa;
		
	srand(time(NULL));
	alarm_time = rand()%10;
	alarm(alarm_time);

	sa.sa_handler = &alarm_flagger;
	sigaction(SIGALRM, &sa, NULL); 

	while(1) {
		printf("%d\n", i);
		if(alarm_flag == 1) {
			alarm_time = rand()%10;
			alarm(alarm_time);
			i = 0;
			alarm_flag = 0;
		}
		i++;
	}
	return 0;
}

