#include <user.h>


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
		pair = &(rps->clientPairs[i]);
		
		pair->clientOne = -1;
		pair->clientTwo = -1;
		
		pair->clientOneChoice = IDLE;
		pair->clientTwoChoice = IDLE;
		
		pair->status = WAITING;		
	}
}

static char* choiceMap(int choice, char * retval){
	switch(choice)
	{
		case 0: 
			retval = "IDLE";
			break;
		case 1:
			retval = "ROCK";
			break;
		case 2: 
			retval = "PAPER";
			break;
		case 3:
			retval = "SCISSORS";
			break;
	}
	return retval;
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
	
	RegisterAs("RPS Server");

	for(;;){
		Receieve(&playerTid, &req, sizeof(RPS_clientRequest));
		switch (req.rpscall_uid){
			case RPSCALL_SIGNUP:{
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
				int matchResult1;
				int matchResult2;	
				int whichTable;
				pair = &(rps.clientPairs[req.whichTable]);
				if (pair->status == QUITTED){
					pair->clientOne = -1;
					pair->clientTwo = -1;
					pair->clientOneChoice = IDLE;
					pair->clientTwoChoice = IDLE;
					pair->status = WAITING;	

					bwprintf( COM2, "Both player on table #%d quit\r\n", req.whichTable);
					whichTable = -1;
					Reply(req.tid, &whichTable, sizeof(int));
				}
				char * temp;
				if (pair->clientOneChoice == IDLE){
					pair->clientOneChoice = req.data;
					bwprintf( COM2, "Player 1 on table #%d chose: %s\r\n", req.whichTable, choiceMap(req.data, temp));
				}
				else if (pair->clientTwoChoice == IDLE){
					pair->clientTwoChoice = req.data;
					bwprintf( COM2, "Player 2 on table #%d chose: %s\r\n", req.whichTable, choiceMap(req.data, temp));
				}

				if (pair->clientOneChoice != IDLE && pair->clientTwoChoice != IDLE){
					if (pair->clientOneChoice == pair->clientTwoChoice){
						matchResult1 = TIE;
						matchResult2 = TIE;	
						bwprintf( COM2, "Table #%d result: %s\r\n", req.whichTable, "TIE");
					}
					else if ((pair->clientOneChoice == pair->clientTwoChoice + 1)
							|| (pair->clientOneChoice == pair->clientTwoChoice - 2)){
						matchResult1 = WIN;
						matchResult2 = LOSE;
						bwprintf( COM2, "Table #%d result: %s\r\n", req.whichTable, "Player 1 WIN");
					}
					else{
						matchResult1 = LOSE;
						matchResult2 = WIN;
						bwprintf( COM2, "Table #%d result: %s\r\n", req.whichTable, "Player 2 WIN");
					}
					pair->clientOneChoice = IDLE;
					pair->clientTwoChoice = IDLE;
					pair->status = WAITING;	
					Reply(pair->clientOne, &matchResult1, sizeof(int));
					Reply(pair->clientTwo, &matchResult2, sizeof(int));
				}

				// error checking to be added
				break;	
			}				
			case RPSCALL_QUIT:
			{
				int whichTable;
				pair = &(rps.clientPairs[req.whichTable]);
				if (pair->status == QUITTED){
					pair->clientOne = -1;
					pair->clientTwo = -1;
					pair->clientOneChoice = IDLE;
					pair->clientTwoChoice = IDLE;
					pair->status = WAITING;
					bwprintf( COM2, "Both player on table #%d quit\r\n", req.whichTable);
				}
				else{
					pair->status = QUITTED;
					bwprintf( COM2, "One player on table #%d quit\r\n", req.whichTable);
				}

				whichTable = -1;
				Reply(req.tid, &whichTable, sizeof(int));
				break;				
			}
		}
		
		
	}
}
