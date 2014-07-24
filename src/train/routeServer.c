#include <train.h>


#define	FREE		0
#define	BLOCKED		1

#define UNFINISHED	0
#define FINISHED	1

#define INFINITY	999999;

#define REVERSE_COST	400		//750*2

typedef struct trainPathInfo{
	int isUsed;

	int trainNo;

	//int currentIdx;
	int trackPathNodes[TRACK_MAX];
}trainPathInfo;


typedef struct routeServerData{
	track_node trackA[TRACK_MAX];
	int	trackNodeStatus[TRACK_MAX];	

	trainPathInfo trainPathInfos[MAX_TRAINS];
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
	
	*cost = 0;
	*isReverse = 0;
	
	switch (src->type)
	{
		case NODE_BRANCH:
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

/*
 *	Reservation related functions
 */


static void
initRouteServerData(routeServerData* rtSvrData){
	int i;
	
	init_tracka(rtSvrData->trackA);
	
	for (i = 0; i < TRACK_MAX; i++)
	{
		rtSvrData->trackNodeStatus[i] = FREE;
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
								
				/*
				 * Dijkstra's Algo: Init Step
				 */ 														
				int i;
				for (i = 0; i < TRACK_MAX; i++)
				{
					dests[i].v = i;
					dests[i].isFinished = UNFINISHED;
					dests[i].D = INFINITY;
					dests[i].p = -1;				
				}
				
				dests[src].D = 0; 		
						
				/*
				 * Dijkstra's Algo: Main loop
				 */ 
				int mineNodeIdx;
				while(1)
				{
					mineNodeIdx = findMin(dests);
						
					dests[mineNodeIdx].isFinished = 1;
					
					if(mineNodeIdx == dest || mineNodeIdx == -1)
						break;
					
					if (rtSvrData.trackNodeStatus[mineNodeIdx] == BLOCKED)
						continue;						
														
					for (i = 0; i < TRACK_MAX; i++)
					{						
						if (dests[i].isFinished == UNFINISHED && 
								isNeighbor(&(rtSvrData.trackA[mineNodeIdx]), &(rtSvrData.trackA[i]), &cost, &isReverse))
							{
								if(dests[mineNodeIdx].D + cost < dests[i].D){							
									dests[i].D = dests[mineNodeIdx].D + cost;
									dests[i].p = dests[mineNodeIdx].v;
							}								
						}						
					}					
				}
				
				trainPath response;
											
				if (mineNodeIdx != -1 && mineNodeIdx != src){
					int curIdx = mineNodeIdx;
					
					int i = 0;
					while(curIdx != src){
						response.path[(TRACK_MAX-1) - i] = curIdx;
						
						curIdx = dests[curIdx].p;
						i++;
					}
					response.path[(TRACK_MAX-1) - i] = curIdx;

					response.path[0] = (TRACK_MAX-1) - i;


				}
				else{
					response.path[0] = -1;
				}
				/*
				int a = 0;	
				for (i = response.path[0]; i < TRACK_MAX; i++)
				{
						sprintf(COM2, "%s\033[45;%uH%d%s", 
							save, a, response.path[i], restore);
						a = a + 6;					
				}
				*/
				Reply(requester, &response, sizeof(trainPath));			
				break;		
			}	
			case ROUTESERVER_TRACKNODE_RELEASE_BYSENSOR:
			{
				Reply(requester, NULL, 0);	

				int trainNo 		= req.src;
				int triggeredSensor = req.dest;

				trainPathInfo* thisTrainPathInfo = &(rtSvrData.trainPathInfos[trainNo-45]);

				
				break;
			}
			case ROUTESERVER_TRACKNODE_RELEASE_BYTRAIN:
			{
				Reply(requester, NULL, 0);	

				int trainNo 		= req.src;


				break;
			}		
		}
	}
}

void
GoTo(int trainNo, int trainSpeed, int dest){
	/*
	 * After this function is finished, tr rv sw will disabled
	 * 
	 * 1. get the current src node from track for this train
	 * 2. ask the routeServer for a shortest path from src to dest
	 * 3. tell the path to the trackServer
	 * 4. trackServer parse this command and formated this path
	 * 5. trackServer pass this formated path to the trainCommandWorker
	 */ 
	 
	locationInfo trainLoc = getTrainLocation(trainNo);	 //Step 1
	//sprintf(COM2, "%s\033[50;0H%s%d,%d%s", 
	//		save, clearLine, trainLoc.sensor, trainLoc.displacement,  restore);	

	/*
	 * Step 2
	 */  	 	 
	routeServerRequest req;			
	trainPath response;
	
	req.rtSvrReq_uid = ROUTESERVER_ROUTE_GET_SHORTEST;
	
	req.src  = (trainLoc.sensor / 17 -'A') * 16 + (trainLoc.sensor % 17) - 1;
	//TODO: parse
	req.dest = (dest / 17 -'A') * 16 + (dest % 17) - 1;

	Send(ROUTESERVER_TID, &req, sizeof(routeServerRequest), &response, sizeof(trainPath));
	response.trainSpeed = trainSpeed;
	/*
	 * Step 3
	 */ 
	executePath(trainNo, &response);	
}

void
releaseNodesBySensor(int trainNo, int triggeredSensor){
	routeServerRequest req;			
	
	req.rtSvrReq_uid = ROUTESERVER_TRACKNODE_RELEASE_BYSENSOR;
	
	req.src = trainNo;
	req.dest = triggeredSensor;

	Send(ROUTESERVER_TID, &req, sizeof(routeServerRequest), NULL, 0);
}
void
releaseNodesByTrainNo(int trainNo){
	routeServerRequest req;			
	
	req.rtSvrReq_uid = ROUTESERVER_TRACKNODE_RELEASE_BYTRAIN;
	
	req.src = trainNo;

	Send(ROUTESERVER_TID, &req, sizeof(routeServerRequest), NULL, 0);
}
