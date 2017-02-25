#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char const *argv[]) {

	char **upper_argv;
	int i = 0, j = 0;
	int upper_argv_len = 0;

	upper_argv = (char **)malloc(sizeof(char *) * argc);

	printf("The upper case argv is: \n");
	for(i=0; i<argc; i++) {
		upper_argv_len = strlen(argv[i]);
		upper_argv[i] = malloc((1+upper_argv_len) * sizeof(char));

		for (j = 0; j < upper_argv_len; j++) {
			upper_argv[i][j] = putchar(toupper(argv[i][j]));
		}
		printf(" ");
	}
	printf("\n");

	return 0;
}
