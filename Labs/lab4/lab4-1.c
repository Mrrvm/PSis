#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>

typedef struct{
		unsigned start, end;
} com_struct;

int main(){
	unsigned int  m_7, m_19, m_7_19;
	m_7 = m_19 = m_7_19 = 0;
	com_struct limits;

	int j = 0;
	int pid;

	for (int i = 0; i < 3; ++i)
	{
		pid = fork();

		if (pid == 0)
		{
			if (j == 0)
			{
				limits.start = 0;
				limits.end = UINT_MAX/3;
			}
			else if (j == 1)
			{
				limits.start = UINT_MAX/3 +1;
				limits.end	= UINT_MAX*2/3;
			}
			else if (j == 2)
			{
				limits.start = UINT_MAX*2/3;
				limits.end = UINT_MAX;
			}

			break;
		}

		j++;

	}


	if (pid == 0)
	{
		for (unsigned int  a = limits.start; a <  limits.end; a++)
		{
			if(a%7 == 0)
				m_7 ++;
			
			if(a%19 == 0) 
				m_19 ++;
			
			if((a%7 == 0)	&& (a%19 == 0))
				m_7_19++;
			
		}

		printf("m 7    %d\n", m_7);
		printf("m   19 %d\n", m_19);
		printf("m 7 19 %d\n\n", m_7_19);

		exit(0);
	}

	for (int b = 0; b < 3; ++b)
	{
		wait(NULL);
	}

	return 0;

}
