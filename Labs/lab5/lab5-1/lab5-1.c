#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>

int main(int argc, char const *argv[]) {
  int m = 0;
  int i = 0;
  int b = 0;
  int m_7_19, m_19, m_7;
  int pid;
  int pip0[2], pip1[2], pip2[2];

  m_7_19= m_19= m_7=0;

  pipe(pip0);
  pipe(pip1);
  pipe(pip2);

  for (i = 0; i < 3; ++i)
  {
    pid = fork();

    if (pid == 0) {
      close(pip0[1]);
      close(pip1[1]);
      close(pip2[1]);
      break;
    }
  }

  if(pid == 0) {
    if(i == 0) {
      while(read(pip0[0], &b, sizeof(b))) {
        if(b%7 == 0){
         m_7 ++;
        }
      }
      close(pip0[0]);
      close(pip1[0]);
      close(pip2[0]);
      printf("m 7    %d\n", m_7);
      exit(0);
    }
    else if(i == 1) {
      while(read(pip1[0], &b, sizeof(b))) {
        if(b%19 == 0){
         m_19 ++;
        }
      }
      close(pip0[0]);
      close(pip1[0]);
      close(pip2[0]);
      printf("m 19    %d\n", m_19);
      exit(0);
    }
    else {
      while(read(pip2[0], &b, sizeof(b))) {
        if((b%19  == 0) && (b%7  == 0)){
             m_7_19 ++;
        }
      }
      close(pip0[0]);
      close(pip1[0]);
      close(pip2[0]);
      printf("m 7 19    %d\n", m_7_19);
      exit(0);
    }
  }

    
  /*Parent!*/
  if(pid != 0) {
    close(pip0[0]);
    close(pip1[0]);
    close(pip2[0]);
    while(m <= 99) {
      write(pip0[1], &m, sizeof(m));
      write(pip1[1], &m, sizeof(m));
      write(pip2[1], &m, sizeof(m));
      m++;
    }
    close(pip0[1]);
    close(pip1[1]);
    close(pip2[1]);
    wait(NULL);
  }
  return 0;
}
