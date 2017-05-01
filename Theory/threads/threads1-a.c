#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
  #include <unistd.h>

	int i = 0;

void * func (void * arg){

	printf("New thread ID %d %lu\n",getpid(), pthread_self()) ;
	while(1){
		i++;
		printf("%d %d %lu\n",i, getpid(), pthread_self()) ;
		sleep(1);
	}

}

int main(){
	pthread_t thr_id_1;
	pthread_t thr_id_2;

	int error;


	error = pthread_create(&thr_id_1, 
							NULL,
							func, NULL);
	if(error != 0){
		perror("pthread_create: ");
		exit(-1);
	}

	error = pthread_create(&thr_id_2, 
							NULL,
							func, NULL);
	if(error != 0){
		perror("pthread_create: ");
		exit(-1);
	}

	printf("Thread ID %d %lu\n",getpid(), pthread_self()) ;
	getchar();
	exit(0);
}
