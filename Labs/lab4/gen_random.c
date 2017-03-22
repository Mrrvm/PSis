#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>
#include <sys/mman.h>

int i = 0;
int fd;

void alarm_flagger(int sig) {
	printf("%d\n", i);
	exit(0);
}

int main(int argc, char const *argv[])
{
	int alarm_time = 10;
	struct sigaction sa;
	int rand_n = 0;
	srand(time(NULL));
	fd = open("mem_file", O_RDWR);
	char *mem = (char *)mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

	alarm(alarm_time);
	sa.sa_handler = &alarm_flagger;
	sigaction(SIGALRM, &sa, NULL); 

	while(1) {
		rand_n = rand()%20;
		sprintf(mem, "%d", rand_n);
		i++;
	}
}