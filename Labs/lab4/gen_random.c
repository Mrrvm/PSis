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

void error_and_die(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

void alarm_flagger(int sig) {
	printf("generated %d random numbers\n", i);
	close(fd);
	exit(0);
}

int main(int argc, char const *argv[])
{
	// shared memory support
	const char *memname = "sample";
  	const size_t region_size = sysconf(_SC_PAGE_SIZE);
  	int r;
  	// alarm support
	int alarm_time = 10;
	struct sigaction sa;
	// number generation support
	int rand_n = 0;
	srand(time(NULL));

	// shared memory settings
	fd = shm_open(memname, O_CREAT | O_TRUNC | O_RDWR, 0666);
	if (fd == -1)
    	error_and_die("shm_open");
 
	r = ftruncate(fd, region_size);
	if (r != 0)
	   error_and_die("ftruncate");
	 
	int *ptr = mmap(0, region_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (ptr == MAP_FAILED)
	    error_and_die("mmap");
 	
 	// alarm settings
	alarm(alarm_time);
	sa.sa_handler = &alarm_flagger;
	sigaction(SIGALRM, &sa, NULL); 

	while(1) {
		rand_n = rand()%20;
		*ptr = rand_n;
		i++;
	}
}

