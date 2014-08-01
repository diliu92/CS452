#ifndef __TRACKSERVER_H__
#define __TRACKSERVER_H__

#include <train/routeServer.h>


#define TRACKSERVER_TID					10


#define TRACKSERVER_INIT_TRAIN				0

#define TRACKSERVER_SPEED_GET 				1
#define TRACKSERVER_SPEED_CHANGE			2

#define TRACKSERVER_SWITCH_GET				3
#define TRACKSERVER_SWITCH_CHANGE			4

#define TRACKSERVER_TRAIN_DELAYOFFSET_GET	5
#define TRACKSERVER_TRAIN_LOCATION_GET		6

#define TRACKSERVER_REVERSE					7

#define TRACKSERVER_UPDATE_LAST_SENSOR		8
#define TRACKSERVER_GET_LAST_SENSOR			9

#define TRACKSERVER_EXECUTE_PATH			10

#define TRACKSERVER_DIST_BETWEEN_SENSORS	11

#define STRAIGHT		33
#define CURVED			34

#define FORWARD			1
#define BACKWARD		2	
	
#define MAX_TRAINS		10
#define MAX_SWITCHES	22
#define MAX_SENSORS 	80

#define TRACK_A_ENTRY_ONE		1
#define TRACK_A_ENTRY_TWO		2
#define TRACK_A_ENTRY_THREE		3
	
	
typedef struct trackServerRequest{		
	int 	trkSvrReq_uid;
	int		retval;
	
	int 	target;
	int 	value;
	int 	value2;
	int 	ts;
	
	trainPath*	path;
}trackServerRequest;

typedef struct locationInfo{	
	int 	sensor;
	int 	displacement;
}locationInfo;


typedef struct trainWorkerCommand{
	enum { RUN, DELAY_STOP, REVERSE} 
		cmdType;
	int cmdValue;
}trainWorkerCommand;
typedef struct trainWorkerRequest{
	int		switchesChange[MAX_SWITCHES];
	
	int 	trainSpeed;

	int		numberOfCommands;
	trainWorkerCommand	commands[20];
}trainWorkerRequest;

void 
trackServer();

void 
initTrain(int trainNo, int entryPoint);

int
getTrainSpeed(int trainNo); 

void 
changeTrainSpeed(int trainNo, int trainSpeed);	//0-15

void
reverseTrain(int trainNo);		//rv

int
getSwitchStatus(int switchNo);

void
changeSwitchStatus(int switchNo, int switchStatus);

void
updateLastTriggeredSensor(int sensorUID, int ts);

void
executePath(int trainNo, trainPath* path);

int
getTrainDelayOffset(int trainNo);

locationInfo
getTrainLocation(int trainNo);



#endif
