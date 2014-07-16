#ifndef __ROUTESERVER_H__
#define __ROUTESERVER_H__

#define ROUTESERVER_TID					11

#define ROUTESERVER_ROUTE_GET_SHORTEST		0		

#define ROUTESERVER_TRACKNODE_CAPTURE		1		//for now, dont need this call 'yet'
#define ROUTESERVER_TRACKNODE_RELEASE		2

#define TRACK_MAX_NUM 144


typedef struct routeServerRequest{		
	int 	rtSvrReq_uid;
	int		retval;
	
	int 	src;
	int 	dest;
}routeServerRequest;

typedef struct trainPath{
	int		path[TRACK_MAX_NUM];
}trainPath;

void
routeServer();


void
GoTo(int trainNo, int dest);
#endif
