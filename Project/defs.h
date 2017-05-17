#ifndef defsHeader
#define defsHeader

#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define RESET "\033[0m"

#define ADD_PHOTO 0
#define ADD_KEYWORD 1
#define SEARCH_PHOTO 2
#define DEL_PHOTO 3
#define GET_NAME 4
#define GET_PHOTO 5

typedef void *item;

typedef struct photo_data_ {
	uint32_t type;
	char *file_name;
	uint32_t id_photo;
	char *keyword;
} photo_data;

#endif