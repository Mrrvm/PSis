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

#define MESSAGE_LEN 100

#define CLI_GW 0 // Client request to Gateway
#define SER_GW 1 // Server sends address to Gateway

#define NO_SER_AVB 0
#define SER_AVB 1

#define MY_IP "192.168.1.8"

typedef struct message_gw{
    int type;
    char address[20];
    int port;
} message_gw;

typedef struct message_std{
	int type;
    char buffer[MESSAGE_LEN];
} message_std;
