#ifndef __ROUTESERVER_H__
#define __ROUTESERVER_H__

#define ROUTESERVER_TID					11

#define ROUTESERVER_ROUTE_GET_SHORTEST				0		

#define ROUTESERVER_TRACKNODE_RELEASE_BYSENSOR		1	//called during train movement
#define ROUTESERVER_TRACKNODE_CAPTURE_BYTRAIN		2	//called when train is stopping

#define TRACK_MAX_NUM 144


typedef struct routeServerRequest{		
	int 	rtSvrReq_uid;
	int		retval;
	
	int 	src;
	int 	dest;
	
	int		trainNo;
	int		trainSpeed;
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
captureNodesByTrainNo(int trainNo);
#endif
