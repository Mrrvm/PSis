#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

int main(int argc, char const *argv[])
{
	execl("./", "./VI.sh", "-c", (char *) NULL);	
	return 0;
}