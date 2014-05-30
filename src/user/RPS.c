#include <user.h>


#define MAX_PAIR_OF_CLIENTS 10

typedef struct clientPair{
	int clientOne;
	int clientTwo;
	
	int clientOneChoice;
	int clientTwoChoice;
	
	int status;
	int roundCount;
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
		pair->roundCount = 0;		
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

void wait(){	
	bwprintf(COM2, "\033[0mPress any key to continue...");
	bwgetc(COM2);
	bwprintf(COM2, "\r\n\r\n");
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
		bwprintf(COM2, "\033[32mRPS Server:\r\n");
		switch (req.rpscall_uid){
			case RPSCALL_SIGNUP:{
				int whichTable;	
							
				for (i = 0; i < MAX_PAIR_OF_CLIENTS; i++)
				{
					pair = &(rps.clientPairs[i]);
					if(rps.clientPairs[i].status == WAITING){
						if(pair->clientOne ==  -1){
							pair->clientOne = req.tid;
							bwprintf(COM2, "Player %d is signed to table #%d, waiting for another player\r\n", req.tid, i);
						}
						else{
							pair->clientTwo = req.tid;
							
							pair->status = PLAYING;
							whichTable = i;
							
							Reply(pair->clientOne, &whichTable, sizeof(int));
							Reply(pair->clientTwo, &whichTable, sizeof(int));
							bwprintf(COM2, "Player %d is signed to table #%d, the other player on this table is player %d\r\n", pair->clientTwo, whichTable, pair->clientOne);
							bwprintf(COM2, "Table #%d status changed to PLAYING\r\n", whichTable);

						}
						wait();
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
					pair->roundCount = 0;	

					bwprintf( COM2, "Player %d on table #%d forced to quit because the other player quitted\r\n", req.tid, req.whichTable);
					whichTable = -1;
					Reply(req.tid, &whichTable, sizeof(int));
				}
				char * temp;
				if (req.tid == pair->clientOne && pair->clientOneChoice == IDLE){
					pair->clientOneChoice = req.data;
					bwprintf( COM2, "Table #%d Round %d - Player %d chose: %s\r\n", req.whichTable, pair->roundCount + 1, pair->clientOne, choiceMap(req.data, temp));
				}
				else if (req.tid == pair->clientTwo && pair->clientTwoChoice == IDLE){
					pair->clientTwoChoice = req.data;
					bwprintf( COM2, "Table #%d Round %d - Player %d chose: %s\r\n", req.whichTable, pair->roundCount + 1, pair->clientTwo, choiceMap(req.data, temp));
				}

				if (pair->clientOneChoice != IDLE && pair->clientTwoChoice != IDLE){
					bwprintf( COM2, "Table #%d Round %d - Result: ", req.whichTable, pair->roundCount + 1);
					if (pair->clientOneChoice == pair->clientTwoChoice){
						matchResult1 = TIE;
						matchResult2 = TIE;	
						bwprintf( COM2, "%s\r\n", "TIE");
					}
					else if ((pair->clientOneChoice == pair->clientTwoChoice + 1)
							|| (pair->clientOneChoice == pair->clientTwoChoice - 2)){
						matchResult1 = WIN;
						matchResult2 = LOSE;
						bwprintf( COM2, "Player %d WIN\r\n", pair->clientOne);
					}
					else{
						matchResult1 = LOSE;
						matchResult2 = WIN;
						bwprintf( COM2, "Player %d WIN\r\n", pair->clientTwo);
					}
					pair->clientOneChoice = IDLE;
					pair->clientTwoChoice = IDLE;
					pair->roundCount++;	
					Reply(pair->clientOne, &matchResult1, sizeof(int));
					Reply(pair->clientTwo, &matchResult2, sizeof(int));
				}
				
				wait();
				// error checking to be added
				break;	
			}				
			case RPSCALL_QUIT:
			{
				int retval = -1;
				pair = &(rps.clientPairs[req.whichTable]);
				if (pair->status == QUITTED){
					pair->clientOne = -1;
					pair->clientTwo = -1;
					pair->clientOneChoice = IDLE;
					pair->clientTwoChoice = IDLE;
					pair->status = WAITING;
					pair->roundCount = 0;	
				}
				else{
					pair->status = QUITTED;
					if (pair->clientOneChoice != IDLE){
						Reply(pair->clientOne, &retval, sizeof(int));
					}
					else if (pair->clientTwoChoice != IDLE){
						Reply(pair->clientTwo, &retval, sizeof(int));
					}
				}

				Reply(req.tid, &retval, sizeof(int));

				bwprintf( COM2, "Player %d on table #%d requested to quit\r\n", req.tid, req.whichTable);
				
				wait();
				break;				
			}
		}
		bwprintf(COM2, "\033[0m");
	}
}
