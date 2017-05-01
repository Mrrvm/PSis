#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>

int main(){
	
	
	int fd;
	
	fd = open("fifo_teste", O_RDONLY);
	if(fd ==-1){
		perror("open ");
		exit(-1);
	}
	printf("fifo just opened\n");
	char c;
	int n;
	while(1){
		n = read(fd, &c, sizeof(char)); 
		if(n<0){
			perror("read ");
			exit(-1);
		}
		if(n == 0){
			printf("just read zewro bytes");
			exit(-1);
		}
		printf("%c\n", c);
		
	}
	
	printf("fifo aberto\n");
	
}
