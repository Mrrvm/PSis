#include "test.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
	int a;
	printf("What version of the functions you whant to use?\n");
	printf("\t1 - Normal    (test1)\n");
	printf("\t2 - Optimized (test2)\n");
	scanf("%d", &a);
	if (a == 1){
		/* load library test1 */

		printf("running the normal versions from \n");
	}else{
		if(a== 2){
			/* load library test2 */

			printf("running the normal versions\n");
		}else{
			printf("Not running anything\n");
			exit(-1);
		}
	}
	/* call func_1 from whichever library was loaded */
	/* call func_2 from whichever library was loaded */
	exit(0);
	
	
}
