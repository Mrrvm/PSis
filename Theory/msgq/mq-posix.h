typedef struct message1{
	int type;
	char msg[200];
}message1;

typedef struct message2{
	int type;
	int value;
}message2;


#define QUEUE_NAME "/msg_queue"
