
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "mq-posix.h"


int main(){
	struct mq_attr queue_attr;
	mqd_t  queue;
	int i;
	int prio;
	char buffer[256];
	message1 * m1;
	message2 * m2;
	
	queue_attr.mq_maxmsg = 10;
	queue_attr.mq_msgsize = 256;

	queue = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0600, &queue_attr);
	if( queue == -1){
		perror("mq_open()");
		exit(-1);
	}
		i = 0;
	while(1){
		
		sleep(1);
		if (mq_receive(queue, buffer, 300, &prio) == -1){
			perror("mq_receive()");
			exit(-1);
		}
		m1 = buffer;
		printf("Received %d %d ", m1->type, prio);
		if(m1->type == 1){
			printf("type 1 %s\n", m1->msg);
		}
		if(m1->type == 2){
			m2 = m1;
			printf("type 2%d\n", m2->value);			
		}
	}	
	exit(0);
}
