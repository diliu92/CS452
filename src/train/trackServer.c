#include <train.h>


typedef struct trainStatus{
	int 	previousTrainSpeed;
	int 	currentTrainSpeed;
	int		lastSpeedChange; 		//Time();
	
	int 	trainDirection;

	int 	lastTriggeredSensor;
	int 	lastTimeStemp;
	int 	expectedSensor;
	int 	expectedSensorTime;
}trainStatus;

typedef struct trackServerData{
	track_node 	trackA[TRACK_MAX];
	int 		trackFraction[MAX_SENSORS];
	int 		trainsActualSpeeds[2][14];			//0->49, 1->50	in cm/s
	
	int			trainsActualStopDistances[2][14];	//0->49, 1->50	in cm
	
	trainStatus trainsStatus[MAX_TRAINS];
	int			switchesStatus[MAX_SWITCHES];
	int 		initTrainNum;
	int 		currentTrain;
}trackServerData;

static void
initTrainsSpecData(trackServerData* trkSvrData){	
	trkSvrData->trainsActualSpeeds[0][0] = 1348; //need to divide by 10000
	trkSvrData->trainsActualSpeeds[0][1] = 8225; 
	trkSvrData->trainsActualSpeeds[0][2] = 13388;
	trkSvrData->trainsActualSpeeds[0][3] = 18065;
	trkSvrData->trainsActualSpeeds[0][4] = 23386;
	trkSvrData->trainsActualSpeeds[0][5] = 28516;
	trkSvrData->trainsActualSpeeds[0][6] = 35332; 
	trkSvrData->trainsActualSpeeds[0][7] = 41806;
	trkSvrData->trainsActualSpeeds[0][8] = 45630; 
	trkSvrData->trainsActualSpeeds[0][9] = 48900; 
	trkSvrData->trainsActualSpeeds[0][10] = 57038; 
	trkSvrData->trainsActualSpeeds[0][11] = 58700; 
	trkSvrData->trainsActualSpeeds[0][12] = 62222; 
	trkSvrData->trainsActualSpeeds[0][13] = 62221;  
	
	trkSvrData->trainsActualSpeeds[1][0] =  945; 
	trkSvrData->trainsActualSpeeds[1][1] =  7482; 
	trkSvrData->trainsActualSpeeds[1][2] =  12923;
	trkSvrData->trainsActualSpeeds[1][3] =  17263; 
	trkSvrData->trainsActualSpeeds[1][4] =  22015; 
	trkSvrData->trainsActualSpeeds[1][5] =  27196; 
	trkSvrData->trainsActualSpeeds[1][6] =  33286; 
	trkSvrData->trainsActualSpeeds[1][7] =  38269; 	//0.91539
	trkSvrData->trainsActualSpeeds[1][8] =  42781; 	//0.93756
	trkSvrData->trainsActualSpeeds[1][9] =  47828; 	//0.97808
	trkSvrData->trainsActualSpeeds[1][10] = 51079; 	//0.89553
	trkSvrData->trainsActualSpeeds[1][11] = 57041; 	//0.97174
	trkSvrData->trainsActualSpeeds[1][12] = 59658; 	//0.95879
	trkSvrData->trainsActualSpeeds[1][13] = 55469;  

	trkSvrData->trainsActualStopDistances[0][0] = -1; 		//need to divide by 100
	trkSvrData->trainsActualStopDistances[0][1] = -1; 
	trkSvrData->trainsActualStopDistances[0][2] = 14917; 
	trkSvrData->trainsActualStopDistances[0][3] = 22583; 
	trkSvrData->trainsActualStopDistances[0][4] = 29167; 
	trkSvrData->trainsActualStopDistances[0][5] = 36167; 
	trkSvrData->trainsActualStopDistances[0][6] = 45833; 
	trkSvrData->trainsActualStopDistances[0][7] = 51083;
	trkSvrData->trainsActualStopDistances[0][8] = 57917; 
	trkSvrData->trainsActualStopDistances[0][9] = 64917; 
	trkSvrData->trainsActualStopDistances[0][10] = 71333; 
	trkSvrData->trainsActualStopDistances[0][11] = 77917; 
	trkSvrData->trainsActualStopDistances[0][12] = 82083; 
	trkSvrData->trainsActualStopDistances[0][13] = 81667;  
	
	trkSvrData->trainsActualStopDistances[1][0] =  -1; 
	trkSvrData->trainsActualStopDistances[1][1] =  -1; 
	trkSvrData->trainsActualStopDistances[1][2] =  13417;
	trkSvrData->trainsActualStopDistances[1][3] =  20500; 
	trkSvrData->trainsActualStopDistances[1][4] =  26417; 
	trkSvrData->trainsActualStopDistances[1][5] =  32833; 
	trkSvrData->trainsActualStopDistances[1][6] =  40417; 
	trkSvrData->trainsActualStopDistances[1][7] =  47333; 
	trkSvrData->trainsActualStopDistances[1][8] =  52500; 
	trkSvrData->trainsActualStopDistances[1][9] =  59000; 
	trkSvrData->trainsActualStopDistances[1][10] = 66250; 
	trkSvrData->trainsActualStopDistances[1][11] = 71833; 
	trkSvrData->trainsActualStopDistances[1][12] = 75250; 
	trkSvrData->trainsActualStopDistances[1][13] = 77667;  

	trkSvrData->trackFraction[0] =  1;
	trkSvrData->trackFraction[1] =  1;
	trkSvrData->trackFraction[2] =  1;
	trkSvrData->trackFraction[3] =  1; 	
	trkSvrData->trackFraction[4] =  1;
	trkSvrData->trackFraction[5] =  1;
	trkSvrData->trackFraction[6] =  1;
	trkSvrData->trackFraction[7] =  1;
	trkSvrData->trackFraction[8] =  1;
	trkSvrData->trackFraction[9] =  1;
	trkSvrData->trackFraction[10] =  1;
	trkSvrData->trackFraction[11] =  1;
	trkSvrData->trackFraction[12] =  1;
	trkSvrData->trackFraction[13] =  1;	
	trkSvrData->trackFraction[14] =  1;
	trkSvrData->trackFraction[15] =  1;
	trkSvrData->trackFraction[16] =  1;
	trkSvrData->trackFraction[17] =  1;
	trkSvrData->trackFraction[18] =  1;
	trkSvrData->trackFraction[19] =  1;
	trkSvrData->trackFraction[20] =  1;
	trkSvrData->trackFraction[21] =  1;
	trkSvrData->trackFraction[22] =  1;
	trkSvrData->trackFraction[23] =  1; 	
	trkSvrData->trackFraction[24] =  1;
	trkSvrData->trackFraction[25] =  1;
	trkSvrData->trackFraction[26] =  1;
	trkSvrData->trackFraction[27] =  1;
	trkSvrData->trackFraction[28] =  1;
	trkSvrData->trackFraction[29] =  1;
	trkSvrData->trackFraction[30] =  1;
	trkSvrData->trackFraction[31] =  1;
	trkSvrData->trackFraction[32] =  1;
	trkSvrData->trackFraction[33] =  1; 	
	trkSvrData->trackFraction[34] =  1;
	trkSvrData->trackFraction[35] =  1;
	trkSvrData->trackFraction[36] =  1;
	trkSvrData->trackFraction[37] =  1;
	trkSvrData->trackFraction[38] =  1;
	trkSvrData->trackFraction[39] =  1;
	trkSvrData->trackFraction[40] =  1;
	trkSvrData->trackFraction[41] =  1;
	trkSvrData->trackFraction[42] =  1;
	trkSvrData->trackFraction[43] =  1;	
	trkSvrData->trackFraction[44] =  1;
	trkSvrData->trackFraction[45] =  1;
	trkSvrData->trackFraction[46] =  1;
	trkSvrData->trackFraction[47] =  1;
	trkSvrData->trackFraction[48] =  1;
	trkSvrData->trackFraction[49] =  1;
	trkSvrData->trackFraction[50] =  1;
	trkSvrData->trackFraction[51] =  1;
	trkSvrData->trackFraction[52] =  1;
	trkSvrData->trackFraction[53] =  1; 	
	trkSvrData->trackFraction[54] =  1;
	trkSvrData->trackFraction[55] =  1;
	trkSvrData->trackFraction[56] =  1;
	trkSvrData->trackFraction[57] =  1;
	trkSvrData->trackFraction[58] =  1;
	trkSvrData->trackFraction[59] =  1;
	trkSvrData->trackFraction[60] =  1;
	trkSvrData->trackFraction[61] =  1;
	trkSvrData->trackFraction[62] =  1;
	trkSvrData->trackFraction[63] =  1; 	
	trkSvrData->trackFraction[64] =  1;
	trkSvrData->trackFraction[65] =  1;
	trkSvrData->trackFraction[66] =  1;
	trkSvrData->trackFraction[67] =  1;
	trkSvrData->trackFraction[68] =  1;
	trkSvrData->trackFraction[69] =  1;
	trkSvrData->trackFraction[70] =  1;
	trkSvrData->trackFraction[71] =  1;
	trkSvrData->trackFraction[72] =  1;
	trkSvrData->trackFraction[73] =  1; 	
	trkSvrData->trackFraction[74] =  1;
	trkSvrData->trackFraction[75] =  1;
	trkSvrData->trackFraction[76] =  1;
	trkSvrData->trackFraction[77] =  1;
	trkSvrData->trackFraction[78] =  1;
	trkSvrData->trackFraction[79] =  1;
}

static void
initTrackServerData(trackServerData* trkSvrData){
	int i; 

	init_tracka(trkSvrData->trackA);
	initTrainsSpecData(trkSvrData);
	
	for (i = 0; i < MAX_TRAINS; i++)
	{
		(trkSvrData->trainsStatus[i]).previousTrainSpeed = 0;
		(trkSvrData->trainsStatus[i]).currentTrainSpeed  = 0;
		(trkSvrData->trainsStatus[i]).lastSpeedChange  = 0;

		(trkSvrData->trainsStatus[i]).trainDirection	= FORWARD;

		(trkSvrData->trainsStatus[i]).lastTriggeredSensor  = 0;
		(trkSvrData->trainsStatus[i]).expectedSensor  = 0;
		(trkSvrData->trainsStatus[i]).expectedSensorTime  = 0;

		
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
	trkSvrData->currentTrain = -1;
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
				trkSvrData.currentTrain = req.target - 45;
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

				sprintf(COM2, "\033[5;70H%s%d", clearLine, req.value % 15);
				
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
					sprintf(COM2, "\033[4;70H%sBackward", clearLine);
				}
				else{
					trkSvrData.trainsStatus[req.target-45].trainDirection = FORWARD;
					sprintf(COM2, "\033[4;70H%sForward", clearLine);
				}
				
				Reply(requester, NULL, 0);
				break;
			}
			case TRACKSERVER_UPDATE_LAST_SENSOR:
			{
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
				else{
					int group = req.value / 17;
					int id = req.value % 17;

					int ds = req.ts / 10 % 10;
					int s = req.ts / 100 % 60;
					int m = req.ts / 6000 % 60;
					int h = req.ts / 360000;
					sprintf(COM2, "%s\033[15;22H%s%s%c%d\033[16;22H%s%d:%d:%d:%d%s%s", 
						save, yellow, clearLine, (char)group, id, clearLine, h, m, s, ds, resetColor, restore);

					int i = trkSvrData.currentTrain;
					trainStatus *trainStat = &(trkSvrData.trainsStatus[i]);

					if (trainStat->expectedSensor > 0){
						int expt = trainStat->expectedSensorTime + trainStat->lastTimeStemp;
						ds = expt / 10 % 10;
						s = expt / 100 % 60;
						m = expt / 6000 % 60;
						h = expt / 360000;
						sprintf(COM2, "%s\033[17;22H%s%s%d:%d:%d:%d%s%s", 
							save, yellow, clearLine, h, m, s, ds, resetColor, restore);

						int diff = expt - req.ts;
						sprintf(COM2, "%s\033[18;22H%s%s%d ticks%s%s", 
							save, yellow, clearLine, diff, resetColor, restore);

						sprintf(COM2, "%s\033[%u;100H%s%d:%d%s", 
							save, 20 + (trainStat->expectedSensor)/2, clearLine,
							trainStat->expectedSensor-1, 10000 - ((diff * 10000) / trainStat->expectedSensorTime), restore);
					}
					
					int index = (id + (group - 'A') * 16) - 1;
					track_node *nextLandmark = trkSvrData.trackA[index].edge[DIR_AHEAD].dest;
					int totalDist = trkSvrData.trackA[index].edge[DIR_AHEAD].dist;

					while(nextLandmark->type != NODE_SENSOR && nextLandmark->type != NODE_EXIT){
						int direction = DIR_AHEAD;
						if(nextLandmark->type == NODE_BRANCH){
							int status = nextLandmark->num <= 18 ? 
								trkSvrData.switchesStatus[nextLandmark->num-1] : 
								trkSvrData.switchesStatus[nextLandmark->num-135];
							switch(status){
								case STRAIGHT:
									direction = DIR_STRAIGHT;
									break;
								case CURVED:
									direction = DIR_CURVED;
									break;
							}
						}
						totalDist += nextLandmark->edge[direction].dist;
						nextLandmark = nextLandmark->edge[direction].dest;
					}

					int speed = trkSvrData.trainsActualSpeeds[i%2][trainStat->currentTrainSpeed-1];

					trainStat->lastTriggeredSensor = req.value;
					trainStat->lastTimeStemp = req.ts;
					trainStat->expectedSensor = nextLandmark->num; 
					int t = (totalDist * 10000 / speed);// * trkSvrData.trackFraction[nextLandmark->num-1] ; // ticks
					trainStat->expectedSensorTime = t;
					
					t = t + req.ts;

					ds = t / 10 % 10;
					s = t / 100 % 60;
					m = t / 6000 % 60;
					h = t / 360000;
					sprintf(COM2, "%s%s\033[13;16H%s%s\033[13;21Hat: %d:%d:%d:%d%s", 
						save, yellow, clearLine, nextLandmark->name, h, m, s, ds, restore);					
				}

				Reply(requester, NULL, 0);
				break;
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

	Delay(200);									//delay 2s

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
updateLastTriggeredSensor(int sensorUID, int ts){
	trackServerRequest req;
	
	req.trkSvrReq_uid = TRACKSERVER_UPDATE_LAST_SENSOR;
		
	req.target 	= NULL;
	req.value	= sensorUID;
	req.ts = ts;
	
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
