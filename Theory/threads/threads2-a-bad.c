#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
  #include <unistd.h>


void * func (void * arg){

	int n = * (int*) arg;
	int i = 0;


	printf("New thread ID %d %lu\n",getpid(), pthread_self()) ;
	while(1){
		i+=n;
		printf("%d %d %lu\n",i, getpid(), pthread_self()) ;
		sleep(1);
	}

}

int main(){
	pthread_t thr_id_1;
	pthread_t thr_id_2;

	int n;
	int error;

	n = 2;
	error = pthread_create(&thr_id_1, 
							NULL,
							func, &n);
	if(error != 0){
		perror("pthread_create: ");
		exit(-1);
	}
	
	n = 3;
	error = pthread_create(&thr_id_2, 
							NULL,
							func, &n);
	if(error != 0){
		perror("pthread_create: ");
		exit(-1);
	}

	printf("Thread ID %d %lu\n",getpid(), pthread_self()) ;
	getchar();
	exit(0);
}
