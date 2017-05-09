#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
  #include <unistd.h>


void * func (void * arg){

	int n = * (int*) arg;
	long i = 0;
	long * ret_aux = malloc(sizeof(long));

	printf("New thread ID %d %lu\n",getpid(), pthread_self()) ;
	for (int k = 0; k < 999999999; k++){
		i+=n;
	}
	*ret_aux = i;
	return ret_aux;
}

int main(){
	pthread_t thr_id_1;
	pthread_t thr_id_2;

	int n[10];
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

	long * ret1;
	long * ret2;
	pthread_join(thr_id_1, (void*)&ret1);
	pthread_join(thr_id_2, (void*)&ret2);
	
	printf("%ld %ld\n", *ret1, *ret2);
	
	getchar();
	exit(0);
}
