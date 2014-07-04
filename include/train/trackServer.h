#ifndef __TRACKSERVER_H__
#define __TRACKSERVER_H__


#define TRACKSERVER_TID					10


#define TRACKSERVER_SPEED_GET 			1
#define TRACKSERVER_SPEED_CHANGE		2

#define TRACKSERVER_SWITCH_GET			3
#define TRACKSERVER_SWITCH_CHANGE		4

#define STRAIGHT		33
#define CURVED			34

#define FORWARD			1
#define BACKWARD		2	
	
#define MAX_TRAINS		10
#define MAX_SWITCHES	22
	
	
typedef struct trackServerRequest{		
	int 	trkSvrReq_uid;
	float	retval;
	
	int 	key;
	float 	value;
}trackServerRequest;
	
void 
trackServer();


float
getTrainSpeed(int trainNo); 
void 
changeTrainSpeed(int trainNo, float trainSpeed);
void
changeTrainDirection(int trainNo);


int
getSwitchStatus(int switchNo);
void
changeSwitchStatus(int switchNo, int switchStatus);


#endif
