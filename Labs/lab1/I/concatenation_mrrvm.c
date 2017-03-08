#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *concatenate(char dest[], char const src[]) {
	int i = 0, j = 0;
	i = strlen(dest);
	for(j = 0; src[j] != '\0'; j++, i++) {
		dest[i] = src[j];
	}
	dest[i] = '\0';
}

int main(int argc, char const *argv[]) {

	size_t result_string_len = 1024;
	int i = 0;
	char *result_str = (char *)malloc(result_string_len);

	for(i = 1; i < argc; i++) {

		if(strlen(result_str)+strlen(argv[i])+1 <= result_string_len) {
			concatenate(result_str, argv[i]);
		}
		else {
			result_string_len += strlen(argv[i])+1024;
			result_str = (char*) realloc(result_str, result_string_len);
			concatenate(result_str, argv[i]);
		}
	}

	printf("\nThere you go buddy: %s\n", result_str);

	return 0;
}
