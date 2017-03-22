#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/mman.h>

typedef struct{
		unsigned start, end, m_7, m_19, m_7_19;
} com_struct;

int main(){
	unsigned int  m_7, m_19, m_7_19;
	m_7 = m_19 = m_7_19 = 0;

	int i = 0;
	int pid;

	com_struct *limits = (com_struct*)mmap(NULL, 3*sizeof(com_struct), PROT_READ|PROT_WRITE, MAP_ANON|MAP_SHARED, -1, 0 );

	limits[0].start = 0;
	limits[0].end = UINT_MAX/3;
	limits[1].start = UINT_MAX/3+1;
	limits[1].end = UINT_MAX/3*2;
	limits[2].start = UINT_MAX/3*2+1;
	limits[2].end = UINT_MAX;


	for (i = 0; i < 3; ++i)
	{
		pid = fork();

		if (pid == 0)
			break;
	}


	if (pid == 0)
	{
		for (unsigned int  a = limits[i].start; a <  limits[i].end; a++)
		{
			if(a%7 == 0)
				m_7 ++;
			
			if(a%19 == 0) 
				m_19 ++;
			
			if((a%7 == 0)	&& (a%19 == 0))
				m_7_19++;
			
		}

		limits[i].m_7 = m_7;
		limits[i].m_19 = m_19;
		limits[i].m_7_19 = m_7_19;

		printf("[Third %d:]\n", i+1);
		printf("m 7    %d\n", m_7);
		printf("m   19 %d\n", m_19);
		printf("m 7 19 %d\n\n", m_7_19);

		exit(0);
	}


	for (int b = 0; b < 3; ++b)
	{
		wait(NULL);
	}

	for (int b = 0; b < 3; ++b)
	{
		m_7 += limits[b].m_7;
		m_19 += limits[b].m_19;
		m_7_19 += limits[b].m_7_19;
	}

	printf("\nTotal m_7: %d \nTotal m_19: %d \nTotal m_7_19: %d\n", m_7, m_19, m_7_19);



	return 0;

}
