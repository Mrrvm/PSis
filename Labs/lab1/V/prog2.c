#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

typedef void (*func)(void);

int main() {
	int a;
	func func_1, func_2;
	printf("What version of the functions you whant to use?\n");
	printf("\t1 - Normal    (test1)\n");
	printf("\t2 - Optimized (test2)\n");
	scanf("%d", &a);
	if (a == 1) {
		/* load library test1 */
		void *lib1 = dlopen("./test1.so", RTLD_LAZY);
		func_1 = dlsym(lib1, "func_1");
		func_2 = dlsym(lib1, "func_2");	
	}
	else {
		if(a== 2) {
			/* load library test2 */
			void *lib2 = dlopen("./test2.so", RTLD_LAZY);
			func_1 = dlsym(lib2, "func_1");
			func_2 = dlsym(lib2, "func_2");
		}
		else {
			printf("Not running anything\n");
			exit(-1);
		}
	}
	/* call func_1 from whichever library was loaded */
	(*func_1)();
	/* call func_2 from whichever library was loaded */
	(*func_2)();
	exit(0);
	
	
}
