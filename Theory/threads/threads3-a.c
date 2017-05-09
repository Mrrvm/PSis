#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void * func (void * arg){

	long n = * (long*) arg;
	long i = 0;


	printf("New thread ID %d %lu\n",getpid(), pthread_self()) ;
	for (long k = 0; k < 999999999; k++){
		i+=n;
	}
	(* (long*) arg) = i;
}

int main(){
	pthread_t thr_id_1;
	pthread_t thr_id_2;

	long n[10];
	int error;

	n[0] = 2;
	error = pthread_create(&thr_id_1, 
							NULL,
							func, &n[0]);
	if(error != 0){
		perror("pthread_create: ");
		exit(-1);
	}
	
	n[1] = 3;
	error = pthread_create(&thr_id_2, 
							NULL,
							func, &n[1]);
	if(error != 0){
		perror("pthread_create: ");
		exit(-1);
	}

	printf("Thread ID %d %lu\n",getpid(), pthread_self()) ;


	while(1){
		printf("%ld %ld\n", n[0], n[1]);
	}
	getchar();
	exit(0);
}
