#ifndef __RPS_H__
#define __RPS_H__


#define WIN		0
#define LOSE	1

#define IDLE		0
#define	ROCK		1
#define	PAPER		2
#define SCISSORS	3

#define WAITING		0
#define PLAYING		1
#define QUITTED		2

#define RPSCALL_SIGNUP 			1
#define RPSCALL_PLAY			2
#define RPSCALL_QUIT			3



typedef struct RPS_clientRequest{
	int rpscall_uid;
	int retval;
	
	int tid;
	int whichTable;		//used to indicate which table they are in(after the call of signup)
	int data;
}RPS_clientRequest;


void
RPS_Server();



#endif
