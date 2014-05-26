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

static int
RPS_FindNextSpot(RPS* rps){
	
}


/*
 * Error Checking: to be added
 */ 
void
RPS_Server(){
	RPS rps;
	RPS_ServerInit(&rps);
	
	int playerTid;
	RPS_clientRequest req;

	clientPair* pair;	
	int i;
	
	for(;;){
		Receieve(&playerTid, &req, sizeof(RPS_clientRequest));
		switch (req.rpscall_uid)
		{		
			case RPSCALL_SIGNUP:
			{
				int whichTable;	
							
				for (i = 0; i < MAX_PAIR_OF_CLIENTS; i++)
				{
					pair = &(rps.clientPairs[i]);
					if(rps.clientPairs[i].status == WAITING){
						if(pair->clientOne ==  -1)
							pair->clientOne = req.tid;				
						else{
							pair->clientTwo = req.tid;
							
							pair->status = PLAYING;
							whichTable = i;
							
							Reply(pair->clientOne, &whichTable, sizeof(int));
							Reply(pair->clientTwo, &whichTable, sizeof(int));
						}
						break;
					}
				} 
				
				// error checking to be added
				break;
			}
			case RPSCALL_PLAY:
			{
				int matchResult;	
				pair = &(rps.clientPairs[req.whichTable]);
							
				if (){
					
				}
				else
				
				
				// error checking to be added
				break;
			}					
			case RPSCALL_QUIT:
				
				break;					
		}
		
	}
}
