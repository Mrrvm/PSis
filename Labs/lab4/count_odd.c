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

int fd = 0;

int main(int argc, char const *argv[])
{
	fd = open("mem_file", O_RDWR);
	char *mem = (char *)mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	int prev, curr, counter;
	prev = curr = counter = 0;

	//curr = prev = atoi(mem);
	while(1) {
		if(prev != curr) {
			if(curr%2 != 0) {
				counter++;
			}
		}
		prev = curr;
		curr = atoi(mem);
	}
	return 0;
}