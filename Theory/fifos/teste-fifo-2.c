#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>

int main(){
	
	
	int fd;
	
	fd = open("fifo_teste", O_WRONLY);
	if(fd ==-1){
		perror("open ");
		exit(-1);
	}
	printf("fifo just opened\n");
	char c;
	int n;
	while(1){
		c = 'a' + random()%23;
		n = write(fd, &c, sizeof(char)); 
		if(n<0){
			perror("write ");
			exit(-1);
		}
		if(n == 0){
			printf("just wrote zewro bytes");
			exit(-1);
		}
		printf("%c\n", c);
		
	}
	
	printf("fifo aberto\n");
	
}
