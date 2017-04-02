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

void error_and_die(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

int main(int argc, char const *argv[])
{
	// shared memory support
	const char *memname = "sample";
  	const size_t region_size = sysconf(_SC_PAGE_SIZE);
  	int r, fd;

  	// number evaluation support
	int prev, curr, counter;
	prev = curr = counter = 0;

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

	curr = 1;
	prev = 0;
	while(prev != curr) {
		if(curr%2 != 0) {
			counter++;
			printf("%d\n", counter);
		}
		prev = curr;	
		curr = *ptr;
	}
	close(fd);
	return 0;
}