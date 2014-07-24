#ifndef __ROUTESERVER_H__
#define __ROUTESERVER_H__

#define ROUTESERVER_TID					11

#define ROUTESERVER_ROUTE_GET_SHORTEST				0		

#define ROUTESERVER_TRACKNODE_RELEASE_BYSENSOR		1
#define ROUTESERVER_TRACKNODE_RELEASE_BYTRAIN		2

#define TRACK_MAX_NUM 144


typedef struct routeServerRequest{		
	int 	rtSvrReq_uid;
	int		retval;
	
	int 	src;
	int 	trainSpeed;
	int 	dest;
}routeServerRequest;

typedef struct trainPath{
	int 	trainSpeed;
	
	int		path[TRACK_MAX_NUM];
}trainPath;

void
routeServer();


void
GoTo(int trainNo, int trainSpeed, int dest);

void
releaseNodesBySensor(int trainNo, int triggeredSensor);

void
releaseNodesByTrainNo(int trainNo);
#endif
