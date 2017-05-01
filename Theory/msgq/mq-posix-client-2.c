#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "mq-posix.h"

int main(){
	mqd_t  queue;
	int i;
	int prio;
	message2 m;
	
	queue = mq_open(QUEUE_NAME,  O_WRONLY);
	if( queue == -1){
		perror("mq_open()");
		exit(-1);
	}
	
	i = 0;
	while(1){
		printf("Sending Message %d ....\n", i);
		m.type = 2;
		m.value = random();
		mq_send(queue, (char *)&m, sizeof(m), 1);
		printf("sent \n");
		i++;
	}	

	exit(0);
}
