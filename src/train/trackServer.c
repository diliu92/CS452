#include <train.h>


typedef struct trainStatus{
	int 	previousTrainSpeed;
	int 	currentTrainSpeed;
	
	int		lastSpeedChange; 		//Time();
	
	int 	trainDirection;
}trainStatus;

typedef struct trackServerData{
	float 		trainsActualStraightLineSpeeds[2][14];			//0->49, 1->50	in cm/s
	float 		trainsActualCurveLineSpeeds[2][14];	
	
	float		trainsActualStraightLineStopDistances[2][14];	//0->49, 1->50	in cm
	
	trainStatus trainsStatus[MAX_TRAINS];
	int			switchesStatus[MAX_SWITCHES];
	int 		lastTriggeredSensor;
	int 		initTrainNum;
}trackServerData;

static void
initTrainsSpecData(trackServerData* trkSvrData){
		
	trkSvrData->trainsActualStraightLineSpeeds[0][0] = 1.348; 
	trkSvrData->trainsActualStraightLineSpeeds[0][1] = 8.225; 
	trkSvrData->trainsActualStraightLineSpeeds[0][2] = 13.388; 
	trkSvrData->trainsActualStraightLineSpeeds[0][3] = 18.065; 
	trkSvrData->trainsActualStraightLineSpeeds[0][4] = 23.386; 
	trkSvrData->trainsActualStraightLineSpeeds[0][5] = 28.516; 
	trkSvrData->trainsActualStraightLineSpeeds[0][6] = 35.332; 
	trkSvrData->trainsActualStraightLineSpeeds[0][7] = 41.806;
	trkSvrData->trainsActualStraightLineSpeeds[0][8] = 45.630; 
	trkSvrData->trainsActualStraightLineSpeeds[0][9] = 48.900; 
	trkSvrData->trainsActualStraightLineSpeeds[0][10] = 57.038; 
	trkSvrData->trainsActualStraightLineSpeeds[0][11] = 58.700; 
	trkSvrData->trainsActualStraightLineSpeeds[0][12] = 62.222; 
	trkSvrData->trainsActualStraightLineSpeeds[0][13] = 62.221;  
	
	trkSvrData->trainsActualStraightLineSpeeds[1][0] =  0.945; 
	trkSvrData->trainsActualStraightLineSpeeds[1][1] =  7.482; 
	trkSvrData->trainsActualStraightLineSpeeds[1][2] =  12.923;
	trkSvrData->trainsActualStraightLineSpeeds[1][3] =  17.263; 
	trkSvrData->trainsActualStraightLineSpeeds[1][4] =  22.015; 
	trkSvrData->trainsActualStraightLineSpeeds[1][5] =  27.196; 
	trkSvrData->trainsActualStraightLineSpeeds[1][6] =  33.286; 
	trkSvrData->trainsActualStraightLineSpeeds[1][7] =  38.269; 
	trkSvrData->trainsActualStraightLineSpeeds[1][8] =  42.781; 
	trkSvrData->trainsActualStraightLineSpeeds[1][9] =  47.828; 
	trkSvrData->trainsActualStraightLineSpeeds[1][10] = 51.079; 
	trkSvrData->trainsActualStraightLineSpeeds[1][11] = 57.041; 
	trkSvrData->trainsActualStraightLineSpeeds[1][12] = 59.658; 
	trkSvrData->trainsActualStraightLineSpeeds[1][13] = 55.469;  

	trkSvrData->trainsActualStraightLineStopDistances[0][0] = -1; 
	trkSvrData->trainsActualStraightLineStopDistances[0][1] = -1; 
	trkSvrData->trainsActualStraightLineStopDistances[0][2] = 14.917; 
	trkSvrData->trainsActualStraightLineStopDistances[0][3] = 22.583; 
	trkSvrData->trainsActualStraightLineStopDistances[0][4] = 29.16l7; 
	trkSvrData->trainsActualStraightLineStopDistances[0][5] = 36.167; 
	trkSvrData->trainsActualStraightLineStopDistances[0][6] = 45.833; 
	trkSvrData->trainsActualStraightLineStopDistances[0][7] = 51.083;
	trkSvrData->trainsActualStraightLineStopDistances[0][8] = 57.917; 
	trkSvrData->trainsActualStraightLineStopDistances[0][9] = 64.917; 
	trkSvrData->trainsActualStraightLineStopDistances[0][10] = 71.333; 
	trkSvrData->trainsActualStraightLineStopDistances[0][11] = 77.917; 
	trkSvrData->trainsActualStraightLineStopDistances[0][12] = 82.083; 
	trkSvrData->trainsActualStraightLineStopDistances[0][13] = 81.667;  
	
	trkSvrData->trainsActualStraightLineStopDistances[1][0] =  -1; 
	trkSvrData->trainsActualStraightLineStopDistances[1][1] =  -1; 
	trkSvrData->trainsActualStraightLineStopDistances[1][2] =  13.417;
	trkSvrData->trainsActualStraightLineStopDistances[1][3] =  20.500; 
	trkSvrData->trainsActualStraightLineStopDistances[1][4] =  26.417; 
	trkSvrData->trainsActualStraightLineStopDistances[1][5] =  32.833; 
	trkSvrData->trainsActualStraightLineStopDistances[1][6] =  40.417; 
	trkSvrData->trainsActualStraightLineStopDistances[1][7] =  47.333; 
	trkSvrData->trainsActualStraightLineStopDistances[1][8] =  52.500; 
	trkSvrData->trainsActualStraightLineStopDistances[1][9] =  59.000; 
	trkSvrData->trainsActualStraightLineStopDistances[1][10] = 66.250; 
	trkSvrData->trainsActualStraightLineStopDistances[1][11] = 71.833; 
	trkSvrData->trainsActualStraightLineStopDistances[1][12] = 75.250; 
	trkSvrData->trainsActualStraightLineStopDistances[1][13] = 77.667;  	
}


static void
initTrackServerData(trackServerData* trkSvrData){
	int i; 
	
	initTrainsSpecData(trkSvrData);
	
	for (i = 0; i < MAX_TRAINS; i++)
	{
		(trkSvrData->trainsStatus[i]).previousTrainSpeed = 0;
		(trkSvrData->trainsStatus[i]).currentTrainSpeed  = 0;
		
		(trkSvrData->trainsStatus[i]).lastSpeedChange  = 0;
		
		(trkSvrData->trainsStatus[i]).trainDirection	= FORWARD;
		
		putc(COM1, 0); 
		putc(COM1, i + 45);
	}
	for (i = 0; i < MAX_SWITCHES; i++)
	{
		if (i <= 17)
		{
			(trkSvrData->switchesStatus[i]) = CURVED;
			
			putc(COM1, CURVED);
			putc(COM1, i + 1);
		}
		else if (i == 18 || i == 20)
		{
			(trkSvrData->switchesStatus[i]) = CURVED;
			
			putc(COM1, CURVED);
			putc(COM1, i + 135);			
		}
		else if (i == 19 || i == 21)
		{
			(trkSvrData->switchesStatus[i]) = STRAIGHT;
			
			putc(COM1, STRAIGHT);
			putc(COM1, i + 135);			
		}
		putc(COM1, 32);				
	}

	trkSvrData->initTrainNum = -1;
	trkSvrData->lastTriggeredSensor = -1;
}

void
trackServer(){
	trackServerData trkSvrData;
	
	int requester;
	trackServerRequest req;
	
	putc(COM1, 96);	
	initTrackServerData(&trkSvrData);
	
	//sprintf(COM2, "\033[6;70H%d", (int)trkSvrData.trainsActualSpeeds[0][7]);
	
	while (1)
	{
		Receive(&requester, &req, sizeof(trackServerRequest));
		
		switch (req.trkSvrReq_uid)
		{
			case TRACKSERVER_INIT_TRAIN:
			{
				trkSvrData.initTrainNum = req.target;
				Reply(requester, NULL, 0);
			}
			case TRACKSERVER_SPEED_GET:
			{			
				Reply(requester, &(trkSvrData.trainsStatus[req.target-45].currentTrainSpeed), sizeof(int));
				break;
			}
			case TRACKSERVER_SPEED_CHANGE:
			{
				trainStatus *targetTrainStatus = &(trkSvrData.trainsStatus[req.target-45]);

				targetTrainStatus->previousTrainSpeed = targetTrainStatus->currentTrainSpeed;
				targetTrainStatus->lastSpeedChange = Time();
				targetTrainStatus->currentTrainSpeed = req.value % 15;

				sprintf(COM2, "\033[5;70H%d", req.value % 15);
				
				// if (req.value == 15){
				// 	targetTrainStatus->trainDirection = 
				// 		(targetTrainStatus->trainDirection == FORWARD) ? BACKWARD : FORWARD;
				// }
							
				Reply(requester, NULL, 0);									
				break;		
			}	
			case TRACKSERVER_SWITCH_GET:
			{
				if(req.target <= 18)
					Reply(requester, &(trkSvrData.switchesStatus[req.target-1]), sizeof(int));
				else
					Reply(requester, &(trkSvrData.switchesStatus[req.target-135]), sizeof(int));					
				break;	
			}
			case TRACKSERVER_SWITCH_CHANGE:
			{
				if(req.target <= 18)
					trkSvrData.switchesStatus[req.target-1] = req.value;
				else
					trkSvrData.switchesStatus[req.target-135] = req.value;
					
				Reply(requester, NULL, 0);									
				break;	
			}
			case TRACKSERVER_REVERSE:
			{
				if (trkSvrData.trainsStatus[req.target-45].trainDirection == FORWARD){
					trkSvrData.trainsStatus[req.target-45].trainDirection = BACKWARD;
					sprintf(COM2, "\033[4;70HBackward");
				}
				else{
					trkSvrData.trainsStatus[req.target-45].trainDirection = FORWARD;
					sprintf(COM2, "\033[4;70HForward");
				}
				
				Reply(requester, NULL, 0);
			}
			case TRACKSERVER_UPDATE_LAST_SENSOR:
			{
				trkSvrData.lastTriggeredSensor = req.value;
				int n = trkSvrData.initTrainNum;
				if (n != -1 && req.value == ('B' * 17 + 2)){
					putc(COM1, 0);
					putc(COM1, n);
					
					Delay(100);

					putc(COM1, 15);
					putc(COM1, n);
					
					putc(COM1, trkSvrData.trainsStatus[n-45].currentTrainSpeed);
					putc(COM1, n);
					trkSvrData.initTrainNum = -1;
				}
				else if (n != -1 && req.value == ('D' * 17 + 14)){
					trkSvrData.initTrainNum = -1;
				}
				Reply(requester, NULL, 0);
			}	
		}
	}
}

int
getTrainSpeed(int trainNo){
	trackServerRequest req;
		
	req.trkSvrReq_uid = TRACKSERVER_SPEED_GET;
		
	req.target = trainNo;
	
	Send(TRACKSERVER_TID, &req, sizeof(trackServerRequest), &(req.retval), sizeof(int));	
	return req.retval;
}
void 
changeTrainSpeed(int trainNo, int trainSpeed){
	trackServerRequest req;
	
	putc(COM1, trainSpeed);
	putc(COM1, trainNo);
	
	req.trkSvrReq_uid = TRACKSERVER_SPEED_CHANGE;
		
	req.target 	= trainNo;
	req.value	= trainSpeed;
	
	Send(TRACKSERVER_TID, &req, sizeof(trackServerRequest), NULL, 0);	
}

void
reverseTrain(int trainNo){
	trackServerRequest req;

	putc(COM1, 0);								//soft-stop
	putc(COM1, trainNo);
	
	//Delay(200);									//delay 2s

	putc(COM1, 15);								//change direction and hard-stop
	putc(COM1, trainNo);
	
	putc(COM1, getTrainSpeed(trainNo));			//go back with prev speed
	putc(COM1, trainNo);

	req.trkSvrReq_uid = TRACKSERVER_REVERSE;
	req.target = trainNo;
	Send(TRACKSERVER_TID, &req, sizeof(trackServerRequest), NULL, 0);
}

int
getSwitchStatus(int switchNo){
	trackServerRequest req;
		
	req.trkSvrReq_uid = TRACKSERVER_SWITCH_GET;
		
	req.target = switchNo;
	
	Send(TRACKSERVER_TID, &req, sizeof(trackServerRequest), &(req.retval), sizeof(int));	
	
	return req.retval;
}
void
changeSwitchStatus(int switchNo, int switchStatus){
	trackServerRequest req;
	
	putc(COM1,switchStatus);
	putc(COM1,switchNo);
	putc(COM1, 32);
	
	req.trkSvrReq_uid = TRACKSERVER_SWITCH_CHANGE;
		
	req.target 	= switchNo;
	req.value	= switchStatus;
	
	Send(TRACKSERVER_TID, &req, sizeof(trackServerRequest), NULL, 0);	
}

void
updateLastTriggeredSensor(int sensorUID){
	trackServerRequest req;
	
	req.trkSvrReq_uid = TRACKSERVER_UPDATE_LAST_SENSOR;
		
	req.target 	= NULL;
	req.value	= sensorUID;
	
	Send(TRACKSERVER_TID, &req, sizeof(trackServerRequest), NULL, 0);
}

void 
initTrain(int trainNo){

	changeTrainSpeed(trainNo, 8);

	trackServerRequest req;
	
	req.trkSvrReq_uid = TRACKSERVER_INIT_TRAIN;	
	req.target 	= trainNo;

	Send(TRACKSERVER_TID, &req, sizeof(trackServerRequest), NULL, 0);
}
