#include <train.h>


#define	FREE		0
#define	BLOCKED		1

#define UNFINISHED	0
#define FINISHED	1

#define INFINITY	0xFFFFFFFF

#define REVERSE_COST	1000		//750*2

typedef struct routeServerData{
	track_node trackA[TRACK_MAX];
	int	blockedTrackNodes[TRACK_MAX];	
}routeServerData;

typedef struct DijkstraEntry{
	int v;			//vertex
	
	int isFinished;	
	
	int D; 		//least_cost:D(v)
	int p;		//pre_node:p(v)
}DijkstraEntry;



/*
 *	DijkstraEntry related functions
 */

static int 
NodeToIdx(track_node* track, track_node* src){
	return track - src;
}
static int
isNeighbor(track_node* src, track_node* dest, int* cost, int* isReverse){	//can src link to dest?
	switch (src->type)
	{
		case NODE_BRANCH:
			//if (src->edge[DIR_STRAIGHT].dest == dest || src->edge[DIR_CURVED].dest == dest || src->reverse == dest)
			//	return 1;
			if (src->edge[DIR_STRAIGHT].dest == dest){
				*cost = src->edge[DIR_STRAIGHT].dist;
				return 1;
			}
			else if (src->edge[DIR_CURVED].dest == dest){
				*cost = src->edge[DIR_CURVED].dist;
				return 1;
			}
			else if (src->reverse == dest){
				*cost = REVERSE_COST;
				*isReverse = 1;
				return 1;
			}
			
			break;
		case NODE_SENSOR:
		case NODE_ENTER:
		case NODE_MERGE:
			//if (src->edge[DIR_AHEAD].dest == dest || src->reverse == dest)
			//	return 1;
			if (src->edge[DIR_AHEAD].dest == dest){
				*cost = src->edge[DIR_AHEAD].dist;
				return 1;
			}
			else if (src->reverse == dest){
				*cost = REVERSE_COST;
				*isReverse = 1;
				return 1;
			}
			break;
		case NODE_EXIT:
			if (src->reverse == dest){
				*cost = REVERSE_COST;
				*isReverse = 1;
				return 1;
			}
			break;			
	}

	return 0;
}
static int
findMin(DijkstraEntry* dests){	
	int retval = -1;
	int minCost = INFINITY;
	
	int i;
	for (i = 0; i < TRACK_MAX; i++)
	{
		if(dests[i].isFinished == FINISHED)
			continue;
		else if(dests[i].D < minCost){
			minCost = dests[i].D;
			retval = dests[i].v;
		}
	}
	
	return retval;
}




static void
initRouteServerData(routeServerData* rtSvrData){
	int i;
	
	init_tracka(rtSvrData->trackA);
	
	for (i = 0; i < TRACK_MAX; i++)
	{
		rtSvrData->blockedTrackNodes[i] = FREE;
	}
	
}

void
routeServer(){
	routeServerData rtSvrData;
	
	int requester;
	routeServerRequest req;
	
	initRouteServerData(&rtSvrData);
		

	while (1)
	{
		Receive(&requester, &req, sizeof(routeServerRequest));
		
		switch (req.rtSvrReq_uid)
		{
			case ROUTESERVER_ROUTE_GET_SHORTEST:
			{	
				DijkstraEntry dests[TRACK_MAX];
				
				int src  = req.src;
				int dest = req.dest;

				int firstReverseFree = 1;
				
				int cost, isReverse;
				
				sprintf(COM2, "%s\033[40;0H%d%s", 
					save, 699, restore);				
				/*
				 * Dijkstra's Algo: Init Step
				 */ 	
				int a = 0;														
				int i;
				for (i = 0; i < TRACK_MAX; i++)
				{
					dests[i].v = i;
					dests[i].isFinished = UNFINISHED;
					
					if(isNeighbor(&(rtSvrData.trackA[src]), &(rtSvrData.trackA[i]), &cost, &isReverse))
					{
						if (firstReverseFree == 1 && isReverse == 1){
							dests[i].D = 0;
							dests[i].p = src;	
							
							firstReverseFree == 0;
						}
						else{
							dests[i].D = cost;
							dests[i].p = src;								
						}	
						sprintf(COM2, "%s\033[%u;70H %d %d %s", 
							save, (a++) % 30 + 20, i, dests[i].D, restore);											
					}
					else
					{
						dests[i].D = INFINITY;
						dests[i].p = -1;							
					}					
				}
				
				dests[src].isFinished = FINISHED; 		
						
				/*
				 * Dijkstra's Algo: Main loop
				 */ 
				int mineNodeIdx;
				while(1)
				{
					mineNodeIdx = findMin(dests);
					sprintf(COM2, "%s\033[%u;75H mineNodeIdx:%d %s", 
							save, (a++) % 30 + 20, mineNodeIdx, restore);						
					dests[mineNodeIdx].isFinished = 1;
					
					if(mineNodeIdx == dest || mineNodeIdx == -1)
						break;
					
					if (rtSvrData.blockedTrackNodes[mineNodeIdx] == BLOCKED)
						continue;						
														
					for (i = 0; i < TRACK_MAX; i++)
					{						
						if (dests[i].isFinished == UNFINISHED && 
								isNeighbor(&(rtSvrData.trackA[mineNodeIdx]), &(rtSvrData.trackA[i]), &cost, &isReverse))
						{
							if(dests[mineNodeIdx].D + cost < dests[i].D){
						sprintf(COM2, "%s\033[%u;70H reached:%d %s", 
							save, (a++) % 30 + 20, i, restore);									
								dests[i].D = dests[mineNodeIdx].D + cost;
								dests[i].p = dests[mineNodeIdx].v;
							}								
						}						
					}					
				}	
				sprintf(COM2, "%s\033[42;0H%d %d%s", 
					save, 799, mineNodeIdx, restore);
									
				if (mineNodeIdx != -1){
					int curIdx = mineNodeIdx;
					while(curIdx != src){
						sprintf(COM2, "%s\033[45;%uH%d%s", 
							save, a, curIdx, restore);
						a = a + 6;
						curIdx = dests[curIdx].p;
					}
				}
				
				
				
				Reply(requester, NULL, 0);			
				break;		
			}			
		}
	}
}

void
GetPath(int src, int dest){
	routeServerRequest req;
	
	req.rtSvrReq_uid = ROUTESERVER_ROUTE_GET_SHORTEST;
	
	req.src  = src;
	req.dest = dest;
	
	Send(ROUTESERVER_TID, &req, sizeof(routeServerRequest), NULL, 0);
	
}
