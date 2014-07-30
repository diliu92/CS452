#include <train.h>


#define	FREE		0
#define	BLOCKED		1

#define UNFINISHED	0
#define FINISHED	1

#define INFINITY	999999;

#define REVERSE_COST	400	

typedef struct trainReservationInfo{
	int isUsed;

	int trainNo;
	
	int src;
	int dest;

	int alongPathNodes[TRACK_MAX];	//free them during movement
	int alongPathNodesNumber;
	//int current;
	
	int nearDestNodes[TRACK_MAX];	//free them when train restart
	int nearDestNodesNumber;
}trainReservationInfo;

typedef struct routeServerData{
	track_node trackA[TRACK_MAX];
	int	trackNodeStatus[TRACK_MAX];	

	trainReservationInfo trainReservationInfos[MAX_TRAINS];
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
NodeToIdx(track_node* node, track_node* src){
	return node - src;
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
findMin(DijkstraEntry* dests, routeServerData* rtSvrData){	
	int retval = -1;
	int minCost = INFINITY;
	
	int i;
	for (i = 0; i < TRACK_MAX; i++)
	{
		if(dests[i].isFinished == FINISHED || rtSvrData->trackNodeStatus[i] == BLOCKED)
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
releaseAllNearDestNodes(routeServerData* rtSvrData, trainReservationInfo* thisTrainReservInfo){
	int nearDestNode;
	
	int i;
	for (i = 0; i < thisTrainReservInfo->nearDestNodesNumber; i++)
	{
		nearDestNode = thisTrainReservInfo->nearDestNodes[i];
		
		rtSvrData->trackNodeStatus[nearDestNode] = FREE;
	}
	
	thisTrainReservInfo->nearDestNodesNumber = 0;	
}
static void
releaseAllAlongPathNodes(routeServerData* rtSvrData, trainReservationInfo* thisTrainReservInfo){
	int pathNode;
	
	int i;
	for (i = 0; i < thisTrainReservInfo->alongPathNodesNumber; i++)
	{
		pathNode = thisTrainReservInfo->alongPathNodes[i];
		
		if(pathNode != -1)
			rtSvrData->trackNodeStatus[pathNode] = FREE;
	}
	
	thisTrainReservInfo->alongPathNodesNumber = 0;	
}
static void
releaseAlongPathNodes(routeServerData* rtSvrData, trainReservationInfo* thisTrainReservInfo, int triggeredSensor){
	int pathNode;
	int isExisted = 0;
	
	int i;
	for (i = 0; i < thisTrainReservInfo->alongPathNodesNumber; i++)
	{
		pathNode = thisTrainReservInfo->alongPathNodes[i];
		
		if (pathNode == triggeredSensor){
			isExisted = 1;
			break;
		}
	}
	
	int a = 0;			
	if (isExisted){
		for (i = 0; i < thisTrainReservInfo->alongPathNodesNumber; i++)
		{
			pathNode = thisTrainReservInfo->alongPathNodes[i];
			
			if(pathNode != -1){
				(rtSvrData->trackNodeStatus)[pathNode] = FREE;
				
				thisTrainReservInfo->alongPathNodes[i] = -1;
				
				if (pathNode == triggeredSensor)
					return;		
			}
		}		
	}
}

static void
reserveAllNearDestNodes(routeServerData* rtSvrData, trainReservationInfo* thisTrainReservInfo){
	
	/*
	 * forward direction
	 */ 	
	int dest = thisTrainReservInfo->dest;
	
	(rtSvrData->trackNodeStatus)[dest] = BLOCKED;
	thisTrainReservInfo->nearDestNodes[thisTrainReservInfo->nearDestNodesNumber] = dest;
	thisTrainReservInfo->nearDestNodesNumber++;

	track_node* destNode_NextNode 			= rtSvrData->trackA[dest].edge[DIR_AHEAD].dest;
	track_node* destNode_NextNode_Reverse	= rtSvrData->trackA[dest].edge[DIR_AHEAD].dest->reverse;
	
	int destNode_NextNode_Idx 			= NodeToIdx(destNode_NextNode, rtSvrData->trackA);
	int destNode_NextNode_Reverse_Idx	= NodeToIdx(destNode_NextNode_Reverse, rtSvrData->trackA);
	
	(rtSvrData->trackNodeStatus)[destNode_NextNode_Idx] = BLOCKED;	
	thisTrainReservInfo->nearDestNodes[thisTrainReservInfo->nearDestNodesNumber] = destNode_NextNode_Idx;
	thisTrainReservInfo->nearDestNodesNumber++;
	
	(rtSvrData->trackNodeStatus)[destNode_NextNode_Reverse_Idx] = BLOCKED;	
	thisTrainReservInfo->nearDestNodes[thisTrainReservInfo->nearDestNodesNumber] = destNode_NextNode_Reverse_Idx;
	thisTrainReservInfo->nearDestNodesNumber++;
	/*
	 * backward direction
	 */ 
	int destReverse = rtSvrData->trackA[dest].reverse->num;
	
	(rtSvrData->trackNodeStatus)[destReverse] = BLOCKED;
	thisTrainReservInfo->nearDestNodes[thisTrainReservInfo->nearDestNodesNumber] = destReverse;		
	thisTrainReservInfo->nearDestNodesNumber++;
	
	track_node* destReverseNode_NextNode 		 = rtSvrData->trackA[destReverse].edge[DIR_AHEAD].dest;
	track_node* destReverseNode_NextNode_Reverse = rtSvrData->trackA[destReverse].edge[DIR_AHEAD].dest->reverse;
	
	int destReverseNode_NextNode_Idx			= NodeToIdx(destReverseNode_NextNode, rtSvrData->trackA);
	int destReverseNode_NextNode_Reverse_Idx	= NodeToIdx(destReverseNode_NextNode_Reverse, rtSvrData->trackA);
		
	(rtSvrData->trackNodeStatus)[destReverseNode_NextNode_Idx] = BLOCKED;
	thisTrainReservInfo->nearDestNodes[thisTrainReservInfo->nearDestNodesNumber] = destReverseNode_NextNode_Idx;	
	thisTrainReservInfo->nearDestNodesNumber++;
	
	(rtSvrData->trackNodeStatus)[destReverseNode_NextNode_Reverse_Idx] = BLOCKED;
	thisTrainReservInfo->nearDestNodes[thisTrainReservInfo->nearDestNodesNumber] = destReverseNode_NextNode_Reverse_Idx;	
	thisTrainReservInfo->nearDestNodesNumber++;
}

static void
initRouteServerData(routeServerData* rtSvrData){
	int i;
	
	init_tracka(rtSvrData->trackA);
	
	for (i = 0; i < TRACK_MAX; i++)
	{
		rtSvrData->trackNodeStatus[i] = FREE;
	}
	
	for (i = 0; i < MAX_TRAINS; i++)
	{
		(rtSvrData->trainReservationInfos[i]).isUsed = 0;
		
		(rtSvrData->trainReservationInfos[i]).trainNo = 45 + i;
		
		(rtSvrData->trainReservationInfos[i]).src = -1;
		(rtSvrData->trainReservationInfos[i]).dest = -1;
		
		(rtSvrData->trainReservationInfos[i]).alongPathNodesNumber = 0;
		(rtSvrData->trainReservationInfos[i]).nearDestNodesNumber  = 0;
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
				/*
				 * release nearDestNodes
				 */ 
				trainReservationInfo* thisTrainReservInfo = &(rtSvrData.trainReservationInfos[req.trainNo - 45]);
				
				releaseAllNearDestNodes(&rtSvrData, thisTrainReservInfo);
				
				int src  = req.src;
				int dest = req.dest;
				
				thisTrainReservInfo->src = src;
				thisTrainReservInfo->dest = dest;
				/*
				 * Dijkstra's Algo: Init Step
				 */				
				
				DijkstraEntry dests[TRACK_MAX];

				int firstReverseFree = 1;
				
				int cost, isReverse;
																		
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
					mineNodeIdx = findMin(dests, &rtSvrData);
						
					dests[mineNodeIdx].isFinished = 1;
					
					if(mineNodeIdx == dest || mineNodeIdx == -1)
						break;					
														
					for (i = 0; i < TRACK_MAX; i++)
					{						
						if (dests[i].isFinished == UNFINISHED 
								&& isNeighbor(&(rtSvrData.trackA[mineNodeIdx]), &(rtSvrData.trackA[i]), &cost, &isReverse))
							{
								if(dests[mineNodeIdx].D + cost < dests[i].D){							
									dests[i].D = dests[mineNodeIdx].D + cost;
									dests[i].p = dests[mineNodeIdx].v;
							}								
						}						
					}					
				}
				
				trainPath response;
				
				sprintf(COM2, "%s\033[51;0H%s%s", 
								save, clearLine, restore);		
											
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
					
					/*
					 * Reserve these pathNodes
					 */ 
					thisTrainReservInfo->alongPathNodesNumber = 0;
					
					int pathNode;
					for (i = response.path[0]; i < TRACK_MAX; i++)
					{
						thisTrainReservInfo->alongPathNodes[thisTrainReservInfo->alongPathNodesNumber] = response.path[i];
						thisTrainReservInfo->alongPathNodesNumber++;		
						
						rtSvrData.trackNodeStatus[response.path[i]] = BLOCKED;	
					}
				}
				else{
					response.path[0] = -1;
					reserveAllNearDestNodes(&rtSvrData, thisTrainReservInfo);
				}
				
				Reply(requester, &response, sizeof(trainPath));			
				break;		
			}	
			case ROUTESERVER_TRACKNODE_RELEASE_BYSENSOR:
			{
				Reply(requester, NULL, 0);	

				int triggeredSensor = req.src;

				trainReservationInfo* thisTrainReservationInfo = &(rtSvrData.trainReservationInfos[req.trainNo-45]);

				releaseAlongPathNodes(&rtSvrData, thisTrainReservationInfo, triggeredSensor);
				
				break;
			}
			case ROUTESERVER_TRACKNODE_CAPTURE_BYTRAIN:
			{
				Reply(requester, NULL, 0);	

				trainReservationInfo* thisTrainReservationInfo = &(rtSvrData.trainReservationInfos[req.trainNo-45]);
				
				releaseAllAlongPathNodes(&rtSvrData, thisTrainReservationInfo);
				reserveAllNearDestNodes(&rtSvrData, thisTrainReservationInfo);
				
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

	/*
	 * Step 2
	 */  	 	 
	routeServerRequest req;			
	trainPath response;
	
	req.rtSvrReq_uid = ROUTESERVER_ROUTE_GET_SHORTEST;
	
	req.src  = (trainLoc.sensor / 17 -'A') * 16 + (trainLoc.sensor % 17) - 1;
	req.dest = (dest / 17 -'A') * 16 + (dest % 17) - 1;
	
	req.trainNo 	= trainNo;
	req.trainSpeed 	= trainSpeed;
	
	Send(GOSERVER_TID, &req, sizeof(routeServerRequest), NULL, 0);
	
	/*

	while(1){
		Send(ROUTESERVER_TID, &req, sizeof(routeServerRequest), &response, sizeof(trainPath));
		
		if(response.path[0] != -1){
			break;
		}
		sprintf(COM2, "%s\033[51;0H%sNo path now-> src:%d dest:%d %s", 
				save, clearLine, req.src, req.dest, restore);
		
		Delay(15);
	}
	*/
	//response.trainSpeed = trainSpeed;
	/*
	 * Step 3
	 */ 
	//executePath(trainNo, &response);	
}

void
releaseNodesBySensor(int trainNo, int triggeredSensor){
	routeServerRequest req;			
	
	req.rtSvrReq_uid = ROUTESERVER_TRACKNODE_RELEASE_BYSENSOR;
	
	req.trainNo = trainNo;
	req.src = triggeredSensor;

	Send(ROUTESERVER_TID, &req, sizeof(routeServerRequest), NULL, 0);
}
void
captureNodesByTrainNo(int trainNo){
	routeServerRequest req;			
	
	req.rtSvrReq_uid = ROUTESERVER_TRACKNODE_CAPTURE_BYTRAIN;
	
	req.trainNo = trainNo;

	Send(ROUTESERVER_TID, &req, sizeof(routeServerRequest), NULL, 0);
}


typedef struct goWorkerStatus{
	int myTid;
	int isReady;
}goWorkerStatus;

typedef struct goServerData{
	goWorkerStatus 		goWorkersStatus[2];	
}goServerData;


static void
goWorker(){
	routeServerRequest req;	
	
	trainPath response;
	while (1)
	{	
		req.rtSvrReq_uid = ROUTESERVER_GOWORKER_READY;
		Send(GOSERVER_TID, &req, sizeof(routeServerRequest), &req, sizeof(routeServerRequest));
		
		while(1){
			Send(ROUTESERVER_TID, &req, sizeof(routeServerRequest), &response, sizeof(trainPath));
			
			if(response.path[0] != -1){
				break;
			}
			sprintf(COM2, "%s\033[51;0H%sNo path now-> src:%d dest:%d %s", 
					save, clearLine, req.src, req.dest, restore);
			
			Delay(35);
		}
		
		response.trainSpeed = req.trainSpeed;
		
		executePath(req.trainNo, &response);		
	}
	
}

void
goServer(){
	goServerData goSvrData;
	
	int i;
	for (i = 0; i < 2; i++){
		goSvrData.goWorkersStatus[i].myTid 		= Create(9,goWorker);
		goSvrData.goWorkersStatus[i].isReady 	= 0;
	}
		
	int requester;
	routeServerRequest req;

	while (1){
		Receive(&requester, &req, sizeof(routeServerRequest));
		switch (req.rtSvrReq_uid)
		{
			case ROUTESERVER_GOWORKER_READY:
			{
				for (i = 0; i < 2; i++){
					if (goSvrData.goWorkersStatus[i].myTid == requester){
						goSvrData.goWorkersStatus[i].isReady = 1;
						break;
					}
				}
				break;				
			}
			case ROUTESERVER_ROUTE_GET_SHORTEST:
			{	
				for (i = 0; i < 2; i++){
					if (goSvrData.goWorkersStatus[i].isReady)
						break;
				}		
				Reply(goSvrData.goWorkersStatus[i].myTid, &req, sizeof(routeServerRequest));		
								
												
				goSvrData.goWorkersStatus[i].isReady = 0;
				
				Reply(requester, NULL, 0);
				break;
			}		
		}
				
	}
	
}
