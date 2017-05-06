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

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define RESET "\033[0m"

#define MESSAGE_LEN 100

#define CLI_GW 0 // Client request to Gateway
#define SER_GW 1 // Server sends address to Gateway

#define NO_SER_AVB 0
#define SER_AVB 1

#define MY_IP "192.168.1.8"
#define GW_IP "192.168.1.8"

#define DEBUG_ON

/*DEBUG_OFF - No printfs
  DEBUG_ON  - Printfs
 */
#ifdef DEBUG_ON
  #define spam(a) printf a
#else
  #define spam(a) (void)0
#endif

typedef void* item;

typedef struct message_gw{
    int type;
    char address[20];
    int port;
} message_gw;

typedef struct message_std{
  int type;
    char buffer[MESSAGE_LEN];
} message_std;

typedef enum
{
    FALSE=0,
    TRUE=1
} bool;

void file_error(char *msg);
void memory_error(char *msg);

#endif