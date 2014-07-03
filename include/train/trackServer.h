#ifndef __TRACKSERVER_H__
#define __TRACKSERVER_H__


#define TRACKSERVER_SPEED_GET 			1
#define TRACKSERVER_SPEED_UPDATE		2

#define TRACKSERVER_SWITCH_GET			3
#define TRACKSERVER_SWITCH_UPDATE		4

#define STRAIGHT		33
#define CURVED			34	
	
#define MAX_TRAINS		10
#define MAX_SWITCHES	22
	
	
typedef struct trackServerRequest{		
	int 	syscall_uid;
	float	retval;
	
	int 	key;
	float 	value;
}trackServerRequest;
	
void 
trackServer();


float
getTrainSpeed(int trainNo); 
void 
updateTrainSpeed(int trainNo, float trainSpeed);

int
getSwitchStatus(int switchNo);
void
updateSwitchStatus(int switchNo, int switchStatus);


#endif
