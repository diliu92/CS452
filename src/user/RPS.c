#include <user.h>
#include <RPS.H>


#define MAX_PAIR_OF_CLIENTS 10

typedef struct clientPair{
	int clientOne;
	int clientTwo;
	
	int clientOneChoice;
	int clientTwoChoice;
	
	int status;
}clientPair;

typedef struct RPS{
	clientPair clientPairs[MAX_PAIR_OF_CLIENTS];
}RPS;


static void
RPS_ServerInit(RPS* rps){
	int i;
	clientPair* pair;
	
	for (i = 0; i < MAX_PAIR_OF_CLIENTS; i++)
	{
		pair = &(rps->clientPairs[i])
		
		pair->clientOne = -1;
		pair->clientTwo = -1;
		
		pair->clientOneChoice = IDLE;
		pair->clientTwoChoice = IDLE;
		
		pair->status = WAITING;		
	}
}



void
RPS_Server(){
	RPS rps;
	RPS_ServerInit(&rps);
	
	int playerTid;
	RPS_clientRequest req;

	for(;;){
		Receieve(&playerTid, &req, sizeof(RPS_clientRequest));
		switch (req.rpscall_uid)
		{
			case RPSCALL_SIGNUP:
				
				break;
			case RPSCALL_PLAY:
				
				break;	
			case RPSCALL_QUIT:
				
				break;					
		}
		
	}
}
