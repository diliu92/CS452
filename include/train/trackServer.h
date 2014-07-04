#ifndef __TRACKSERVER_H__
#define __TRACKSERVER_H__


#define TRACKSERVER_TID					10


#define TRACKSERVER_SPEED_GET 			1
#define TRACKSERVER_SPEED_CHANGE		2

#define TRACKSERVER_SWITCH_GET			3
#define TRACKSERVER_SWITCH_CHANGE		4

#define TRACKSERVER_REVERSE				5

#define STRAIGHT		33
#define CURVED			34

#define FORWARD			1
#define BACKWARD		2	
	
#define MAX_TRAINS		10
#define MAX_SWITCHES	22
	
	
typedef struct trackServerRequest{		
	int 	trkSvrReq_uid;
	float	retval;
	
	int 	target;
	int 	value;
}trackServerRequest;
	
void 
trackServer();


int
getTrainSpeed(int trainNo); 
void 
changeTrainSpeed(int trainNo, int trainSpeed);	//0-15

void
reverseTrain(int trainNo);						//rv

int
getSwitchStatus(int switchNo);
void
changeSwitchStatus(int switchNo, int switchStatus);

void
updateLastTriggeredSensor(int sensorUID);
#endif
