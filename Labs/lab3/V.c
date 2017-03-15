#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

int main(int argc, char const *argv[])
{
	int i = 0, alarm_time = 0;
	srand(time(NULL));

	alarm_time = (rand())%10;
	alarm(alarm_time);

	while(1) {
		printf("%d\n", i);
		if(!sigaction(SIGALRM, NULL, NULL)) {
			alarm_time = (rand()^i)%10;
			alarm(alarm_time);
			i = 0;
		}
		i++;
	}
	return 0;
}