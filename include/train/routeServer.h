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

typedef struct routeServerResponse_Path{
	int		path[TRACK_MAX_NUM];
}routeServerResponse_Path;




typedef struct nextSensorInfo{
	char *nextSensorLabel;
	int dist;
	int expectedTime;
}nextSensorInfo;

nextSensorInfo
predictNextSensor(int curSensor, int ts);
#endif
