#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>

int verify_prime(int n){
  int i;
  for(i=2; i <n; i++){
    if(n%i==0)
      return 0;
  }
  return 1;
}
int main(int argc, char const *argv[]) {
  int n_child;
  int _random = 0;
  srand(time(NULL));
  if (argc == 1){
    n_child = 0;
  }
  if(argc == 2){
    if (sscanf(argv[1], "%d", &n_child) == 0)
      n_child = 0;
  }

  printf("number of children %d\n", n_child);


  if(n_child==0){
    int i, n;
    for(i = 2; i < 999999; i++){
      n = random()%999999;
      if (verify_prime(n))
        printf("%d is prime, processed by %d\n", n, getpid());
    }
  }
  else{
    int m = 0;
    int n = 0;
    int pid = 0;
    int pip[2];
    pipe(pip);
    //create n_child processes
    for(m = 0; m < n_child; m++) {
      pid = fork();
      if(pid == 0) {
        close(pip[1]);
        break;
      }     
    }
    //generate random numbers
    if(pid != 0) {
      close(pip[0]);  
      int i ;
      for(i = 2; i < 999999; i++){
        _random = random()%999999;
        write(pip[1], &_random, sizeof(_random));   
      }
      close(pip[1]);
      for (i = 0; i < n_child; ++i)
      {
        wait(NULL);   
      }
    }

    if(pid == 0) {
      while(read(pip[0], &n, sizeof(n))) {
        if(verify_prime(n)) {
          printf("%d is prime, processed by %d\n", n, getpid()); 
        }
      }
      close(pip[0]);
      exit(0);
    }
  }

  return 0;
}
