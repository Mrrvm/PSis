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
    int total_primes = 0;
    for(i = 2; i < 99999; i++){
      n = random()%99999;
      if (verify_prime(n))
        total_primes++;
    }
    printf("%d counted %d total primes\n", getpid(), total_primes);
  }
  else{
    int m = 0;
    int n = 0;
    int pid = 0;
    int pip[2];
    int pip2[2];
    pipe(pip);
    pipe(pip2);

    //create n_child processes
    for(m = 0; m < n_child; m++) {
      pid = fork();
      if(pid == 0) {
        close(pip[1]);
        close(pip2[0]);
        break;
      }     
    }
    //generate random numbers
    if(pid != 0) {
      close(pip[0]);
      close(pip2[1]);
      int i ;
      int total_primes = 0;
      int partial_primes = 0;

      for(i = 2; i < 99999; i++){
        _random = random()%99999;
        write(pip[1], &_random, sizeof(_random));   
      }
      close(pip[1]);

      while(read(pip2[0], &partial_primes, sizeof(partial_primes))){
        total_primes += partial_primes;
      }

      for (i = 0; i < n_child; ++i)
      {
        wait(NULL);   
      }

      printf("Total number of primes: %d\n", total_primes);
    }

    if(pid == 0) {
      int partial_primes = 0;
      while(read(pip[0], &n, sizeof(n))) {
        if(verify_prime(n)) {
          partial_primes++; 
        }
      }
      printf("%d counted %d primes\n", getpid(), partial_primes);
      write(pip2[1], &partial_primes, sizeof(partial_primes));


      close(pip[0]);
      close(pip2[1]);
      exit(0);
    }
  }

  return 0;
}
