#include <train.h>


typedef struct trainStatus{
	int		isUsed;
	int 	trainNum;
	
	int 	previousTrainSpeed;
	int 	currentTrainSpeed;
	int		lastSpeedChange; 		//Time();
	
	int 	trainDirection;

	int 	lastTriggeredSensor;
	int 	lastTimeStemp;
	int 	expectedSensor;
	int 	distToExpectedSensor;
	int 	expectedSensorTime;

	locationInfo 	destInfo;
	int 			needToCheckStopTime;
}trainStatus;

typedef struct trackServerData{
	track_node 	trackA[TRACK_MAX];
	
	int 		trainsActualSpeeds[2][14];			//0->49, 1->50	in (mm/10ms * 10000)
	int			trainsStopDistances[2][14];			//0->49, 1->50	in mm
	int 		trainsStopTimes[2][14];
	int 		trainsStopDeceleration[2][14];
	
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
	
	trkSvrData->trainsActualSpeeds[1][0] =  945; 		//for tr50, works ok with tr48
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

	trkSvrData->trainsStopDistances[0][0] = -1; 		//in mm
	trkSvrData->trainsStopDistances[0][1] = -1; 
	trkSvrData->trainsStopDistances[0][2] = 149; 
	trkSvrData->trainsStopDistances[0][3] = 226; 
	trkSvrData->trainsStopDistances[0][4] = 292; 
	trkSvrData->trainsStopDistances[0][5] = 362; 
	trkSvrData->trainsStopDistances[0][6] = 458; 
	trkSvrData->trainsStopDistances[0][7] = 511;
	trkSvrData->trainsStopDistances[0][8] = 579; 
	trkSvrData->trainsStopDistances[0][9] = 649; 
	trkSvrData->trainsStopDistances[0][10] = 713; 
	trkSvrData->trainsStopDistances[0][11] = 779; 
	trkSvrData->trainsStopDistances[0][12] = 821; 
	trkSvrData->trainsStopDistances[0][13] = 816;  
	
	trkSvrData->trainsStopDistances[1][0] =  -1; 
	trkSvrData->trainsStopDistances[1][1] =  -1; 
	trkSvrData->trainsStopDistances[1][2] =  134;
	trkSvrData->trainsStopDistances[1][3] =  205; 
	trkSvrData->trainsStopDistances[1][4] =  264; 
	trkSvrData->trainsStopDistances[1][5] =  328; 
	trkSvrData->trainsStopDistances[1][6] =  404; 
	trkSvrData->trainsStopDistances[1][7] =  473; 
	trkSvrData->trainsStopDistances[1][8] =  525; 
	trkSvrData->trainsStopDistances[1][9] =  590; 
	trkSvrData->trainsStopDistances[1][10] = 662; 
	trkSvrData->trainsStopDistances[1][11] = 718; 
	trkSvrData->trainsStopDistances[1][12] = 752; 
	trkSvrData->trainsStopDistances[1][13] = 777;  

	int i, j;
	for(i = 0; i < 2; i++){
		for(j = 0; j < 14; j++){
			int vi = trkSvrData->trainsActualSpeeds[i][j];
			int va = vi / 2;
			int stopDist = trkSvrData->trainsStopDistances[i][j];
			trkSvrData->trainsStopTimes[i][j] = stopDist * 10000 / va;
			trkSvrData->trainsStopDeceleration[i][j] = vi / trkSvrData->trainsStopTimes[i][j];
		}
	}
	
}

static void
initTrackServerData(trackServerData* trkSvrData){
	int i; 

	init_tracka(trkSvrData->trackA);
	initTrainsSpecData(trkSvrData);
	
	for (i = 0; i < MAX_TRAINS; i++)
	{
		(trkSvrData->trainsStatus[i]).isUsed = 0; 
		(trkSvrData->trainsStatus[i]).trainNum = i + 45; 		
		
		(trkSvrData->trainsStatus[i]).previousTrainSpeed = 0;
		(trkSvrData->trainsStatus[i]).currentTrainSpeed  = 0;
		(trkSvrData->trainsStatus[i]).lastSpeedChange  = 0;

		(trkSvrData->trainsStatus[i]).trainDirection	= FORWARD;

		(trkSvrData->trainsStatus[i]).lastTriggeredSensor  = -1;
		(trkSvrData->trainsStatus[i]).lastTimeStemp  = -1;
		(trkSvrData->trainsStatus[i]).expectedSensor  = -1;
		(trkSvrData->trainsStatus[i]).expectedSensorTime  = -1;

		(trkSvrData->trainsStatus[i]).needToCheckStopTime  = -1;
		(trkSvrData->trainsStatus[i]).destInfo.sensor = 0;
		(trkSvrData->trainsStatus[i]).destInfo.displacement = 0;
		
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


static track_node*
getNextSensorNode(track_node* curSensorNode, int* totalDist, int* switchesStatus){
	track_node* nextLandmark = curSensorNode;
	
	while(nextLandmark->type != NODE_SENSOR && nextLandmark->type != NODE_EXIT){
		int direction = DIR_AHEAD;
		if(nextLandmark->type == NODE_BRANCH){
			int status = (nextLandmark->num <= 18) ? switchesStatus[(nextLandmark->num)-1] : 
														switchesStatus[(nextLandmark->num)-135] ;
								
			//sprintf(COM2, "%s\033[45;0H%s%d:%d:%d%s", save, clearLine, curSensorNode->num, nextLandmark->num, status, restore);
			switch(status){
				case STRAIGHT:
					direction = DIR_STRAIGHT;
					break;
				case CURVED:
					direction = DIR_CURVED;
					break;
			}
		}
		*totalDist += nextLandmark->edge[direction].dist;
		nextLandmark = nextLandmark->edge[direction].dest;
	}
	return nextLandmark;
}
static int
determineTrainByTriggeredSensor(int TriggeredSensor, trainStatus* trainsStatus){	
	int i;
	for (i = 0; i <MAX_TRAINS; i++)
	{
		if (trainsStatus[i].isUsed == 1 &&
				trainsStatus[i].expectedSensor == TriggeredSensor)
			return i;
	}
	
	return -1;
}


void
trackServer(){
	trackServerData trkSvrData;
	
	int requester;
	trackServerRequest req;
	
	putc(COM1, 96);	
	initTrackServerData(&trkSvrData);
	
	
	while (1)
	{
		Receive(&requester, &req, sizeof(trackServerRequest));
		
		switch (req.trkSvrReq_uid)
		{
			case TRACKSERVER_INIT_TRAIN:
			{
				trkSvrData.initTrainNum = req.target;
				trkSvrData.currentTrain = req.target - 45;
				trkSvrData.trainsStatus[req.target - 45].isUsed = 1;
				
				Reply(requester, NULL, 0);	
				break;
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

				sprintf(COM2, "%s\033[5;70H%s%d%s", save, clearLine, req.value % 15, restore);
					
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
					sprintf(COM2, "%s\033[4;70H%sBackward%s", save, clearLine, restore);
				}
				else{
					trkSvrData.trainsStatus[req.target-45].trainDirection = FORWARD;
					sprintf(COM2, "%s\033[4;70H%sForward%s", save, clearLine, restore);
				}
				
				Reply(requester, NULL, 0);
				break;
			}
			case TRACKSERVER_UPDATE_LAST_SENSOR:
			{
				if (trkSvrData.initTrainNum != -1){
					trainStatus* thisTrainStat = &(trkSvrData.trainsStatus[trkSvrData.initTrainNum - 45]);
					
					thisTrainStat->lastTriggeredSensor = req.value;
					thisTrainStat->lastTimeStemp = 	req.ts;						

					if(	req.value == 	(16 + 10 - 1) 	//B10
						|| req.value == (16 + 12 - 1) 	//B12
						|| req.value == (16 +  8 - 1) 	//B8
					){											
						switch (req.value)
						{
							case (16 + 10 - 1) :
								thisTrainStat->expectedSensor = 5 - 1;  	//A5
								
								break;
							case (16 + 12 - 1) :
								thisTrainStat->expectedSensor = 8 - 1; 		//A8					
								
								break;
							case (16 +  8 - 1) :
								thisTrainStat->expectedSensor = 10 - 1;  	//A10
																
								break;	
						}
									
						putc(COM1, 0);
						putc(COM1, trkSvrData.initTrainNum);
						
						Delay(100);

						putc(COM1, 15);
						putc(COM1, trkSvrData.initTrainNum);
						
						putc(COM1, thisTrainStat->currentTrainSpeed);
						putc(COM1, trkSvrData.initTrainNum);
						
						trkSvrData.initTrainNum = -1;
					}
					else if( req.value == 		( 5 - 1) 			//A5
								|| req.value == ( 8 - 1) 			//A8
								|| req.value == (10 - 1) 		//A10
					){						
						thisTrainStat->expectedSensor = 32 + 7 - 1; 	//C7
						
						trkSvrData.initTrainNum = -1;
					}				
				}
				else {					
					int sensorGroup = req.value / 16 + 'A';
					int sensorId = req.value % 16 + 1;
					
					int ds = req.ts / 10 % 10;
					int s = req.ts / 100 % 60;
					int m = req.ts / 6000 % 60;
					int h = req.ts / 360000;
					sprintf(COM2, "%s\033[15;22H%s%s%c%d\033[16;22H%s%d:%d:%d:%d%s%s", 
						save, yellow, clearLine, (char)sensorGroup, sensorId, clearLine, h, m, s, ds, resetColor, restore);

					int sensorIndex = req.value;

					int trainIdx = determineTrainByTriggeredSensor(sensorIndex, trkSvrData.trainsStatus);	//expected sensor handling(normal case)
									
					if (trainIdx == -1){
						for (trainIdx = 0; trainIdx <MAX_TRAINS; trainIdx++)
						{
							if (trkSvrData.trainsStatus[trainIdx].isUsed == 1){
								int expectedSensor = trkSvrData.trainsStatus[trainIdx].expectedSensor;
								
								sprintf(COM2, "%s\033[44;0H%s expectedSensor:%d %s", save, clearLine, expectedSensor, restore);
								
								track_node*	expectedSensorNode 	= &(trkSvrData.trackA[expectedSensor]);
								
								int 		temp;
								track_node* nextSensorNode = getNextSensorNode(expectedSensorNode, &temp, trkSvrData.switchesStatus);	//forward direction
								//track_node* backSensorNode = expectedSensorNode.reverse;	//backward direction
								
								int nextSensor = nextSensorNode->num;
								//int backSensor = ((backSensorNode->num / 16) + 'A') * 17 + ((backSensorNode->num % 16) + 1);	
								
								sprintf(COM2, "%s\033[45;0H%s nextSensor:%d %s", save, clearLine, nextSensor, restore);
								
								if (req.value == nextSensor){
									sprintf(COM2, "%s\033[46;0H%s :%d: %s", save, clearLine, trainIdx, restore);
									break;
								}
									
								//track_node* backSensorNode = expectedSensorNode.reverse;				//backward direction
								
								
									
								//if (req.value == backSensor)
								//	break;		
							}						
						}	
					}
					if (trainIdx == MAX_TRAINS){
						//sprintf(COM2, "%s\033[45;0H%s XXXXXXXXXX %s", save, clearLine, restore);
					}
									
					trainStatus *trainStat = &(trkSvrData.trainsStatus[trainIdx]);
					
					/*
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
					*/
					
					track_node*	curSensorNode 	= trkSvrData.trackA[sensorIndex].edge[DIR_AHEAD].dest;
					int 		totalDist 		= trkSvrData.trackA[sensorIndex].edge[DIR_AHEAD].dist;

					track_node* nextSensorNode = getNextSensorNode(curSensorNode, &totalDist, trkSvrData.switchesStatus);					

					int speed = trkSvrData.trainsActualSpeeds[trainIdx%2][trainStat->currentTrainSpeed-1];

					trainStat->lastTriggeredSensor = sensorIndex;
					trainStat->lastTimeStemp = req.ts;
					trainStat->expectedSensor = nextSensorNode->num; //((nextSensorNode->num / 16) + 'A') * 17 + ((nextSensorNode->num % 16) + 1); 
					trainStat->distToExpectedSensor = totalDist;
					int t = (totalDist * 10000 / speed);// * trkSvrData.trackFraction[nextLandmark->num-1] ; // ticks
					trainStat->expectedSensorTime = t;
					
					t = t + req.ts;

					ds = t / 10 % 10;
					s = t / 100 % 60;
					m = t / 6000 % 60;
					h = t / 360000;
					sprintf(COM2, "%s%s\033[13;16H%s%s\033[13;21Hat: %d:%d:%d:%d%s", 
						save, yellow, clearLine, nextSensorNode->name, h, m, s, ds, restore);			
					/*
					if (trainStat->destInfo.sensor > 'A'*17){
						int stopDist = trkSvrData.trainsStopDistances[i%2][trainStat->currentTrainSpeed-1];
						int additionalLength = (trainStat->trainDirection == FORWARD) ? 140 : 20;
						int tempSensorId = ((nextLandmark->num / 16) + 'A') * 17 + ((nextLandmark->num % 16) + 1);
						
						int a = 0;
						while (!(nextLandmark->type != NODE_EXIT &&
								tempSensorId == trainStat->destInfo.sensor && 			//sensor is dest sensor
								totalDist + trainStat->destInfo.displacement > stopDist + additionalLength)){					
							
							totalDist += nextLandmark->edge[DIR_AHEAD].dist;
							nextLandmark = nextLandmark->edge[DIR_AHEAD].dest;

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
									a++;
								}

								totalDist += nextLandmark->edge[direction].dist;
								nextLandmark = nextLandmark->edge[direction].dest;
							}							
							tempSensorId = ((nextLandmark->num / 16) + 'A') * 17 + ((nextLandmark->num % 16) + 1);
						}

						int temp = totalDist + trainStat->destInfo.displacement - stopDist - additionalLength;
						int delay = (temp * 10000 / speed) + a - ((trainStat->currentTrainSpeed - 10) / 2);
						int stopSignalTime = delay + trainStat->lastTimeStemp;

						if (delay > 0 && 
							delay < trkSvrData.trainsStopTimes[i%2][trainStat->currentTrainSpeed-1]){
							int tid;
							if (i % 2 == 0){
								tid = WhoIs("stopTrain1_Worker");
							}
							else{
								tid = WhoIs("stopTrain2_Worker");
							}
							trainStat->destInfo.sensor = 0;
							trainStat->destInfo.displacement = 0;
							Send(tid, &stopSignalTime, sizeof(int), NULL, 0);
						}
					}
					*/
				}
				
				Reply(requester, NULL, 0);
				break;
			}
			case TRACKSERVER_TRAIN_LOCATION_GET:
			{
				int curVelocity;
				int displacement;
				//int i = req.target-45;	//for specific train
				int i = trkSvrData.currentTrain; 	//for demo 1
				int sensorTimeStemp = trkSvrData.trainsStatus[i].lastTimeStemp;
				locationInfo locInfo;

				int n = trkSvrData.trainsStatus[i].currentTrainSpeed;
				if (n > 0){
					curVelocity = trkSvrData.trainsActualSpeeds[i%2][n-1];
					displacement = (Time() - sensorTimeStemp) * curVelocity / 10000;
				}
				else{
					int m = trkSvrData.trainsStatus[i].previousTrainSpeed;
					int prevVelocity = trkSvrData.trainsActualSpeeds[i%2][m-1];
					curVelocity = prevVelocity / 2;
					int stopDist = trkSvrData.trainsStopDistances[i%2][m-1];
					int stopTime = trkSvrData.trainsStopTimes[i%2][m-1];
					int deceleration = trkSvrData.trainsStopDeceleration[i%2][m-1];
					int speedChangeTime = trkSvrData.trainsStatus[i].lastSpeedChange;

					// sprintf(COM2, "%s\033[44;0H%sm = %d; pV = %d; cV = %d; sD = %d; sT = %d; sCT = %d; a = -%d %s", 
					// 		save, clearLine, m, prevVelocity, curVelocity, stopDist, stopTime, speedChangeTime, deceleration, restore);

					if (sensorTimeStemp > speedChangeTime){ // stop before this sensor
						//d = vi*t + (1/2)at^2
						int t = sensorTimeStemp - speedChangeTime;
						displacement = stopDist - (t * prevVelocity / 10000 - deceleration * t * t / 2 / 10000);
						// sprintf(COM2, "%s\033[46;0H%s %d - %d * %d * %d / 2 / 10000 = %d%s", 
						// 	save, clearLine, stopDist, deceleration, t, t, displacement, restore);
					}
					else{	//stop after this sensor
						int t = Time();
						int x = (speedChangeTime - sensorTimeStemp);
						int y = (t - speedChangeTime) > stopTime ? stopTime : (t - speedChangeTime);

						//d = vi*t + (1/2)at^2
						displacement = (x * prevVelocity / 10000) + (y * prevVelocity / 10000 - deceleration * y * y / 2 / 10000);
						// sprintf(COM2, "%s\033[48;0H%s (%d * %d / 10000) + (%d * %d / 10000 - %d * %d * %d / 2 / 10000) = %d%s", 
						// 	save, clearLine, x, prevVelocity, y, prevVelocity, deceleration, y, y, displacement, restore);
					}
				}

				displacement += (trkSvrData.trainsStatus[i].trainDirection == FORWARD) ? 140 : 20;
				locInfo.displacement = displacement;
				locInfo.sensor = trkSvrData.trainsStatus[i].lastTriggeredSensor;
				
				Reply(requester, &locInfo, sizeof(locationInfo));
				break;
			}
			case TRACKSERVER_GO_TO_POSITION:
			{

				trainStatus *targetTrainStatus = &(trkSvrData.trainsStatus[req.target-45]);
				targetTrainStatus->destInfo.sensor = req.value;
				targetTrainStatus->destInfo.displacement = req.value2;
				targetTrainStatus->needToCheckStopTime = 1;

				req.retval = 0;
				Reply(requester, &(req.retval), sizeof(int));
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
	req.value	= (sensorUID % 17 + (sensorUID / 17 - 'A') * 16) - 1;
	req.ts = ts;
	
	Send(TRACKSERVER_TID, &req, sizeof(trackServerRequest), NULL, 0);
}

void 
initTrain(int trainNo){
	changeTrainSpeed(trainNo, 10);

	trackServerRequest req;
	
	req.trkSvrReq_uid = TRACKSERVER_INIT_TRAIN;	
	req.target 	= trainNo;

	Send(TRACKSERVER_TID, &req, sizeof(trackServerRequest), NULL, 0);
}

locationInfo
getTrainLocation(int trainNo){
	locationInfo response;
	trackServerRequest req;
	
	req.trkSvrReq_uid = TRACKSERVER_TRAIN_LOCATION_GET;
		
	req.target 	= trainNo;

	Send(TRACKSERVER_TID, &req, sizeof(trackServerRequest), &response, sizeof(locationInfo));
	
	return response;
}

int 
goToPosition(int trainNo, locationInfo destInfo){
	trackServerRequest req;
	
	req.trkSvrReq_uid = TRACKSERVER_GO_TO_POSITION;
	req.target 	= trainNo;
	req.value = destInfo.sensor;
	req.value2 = destInfo.displacement;

	Send(TRACKSERVER_TID, &req, sizeof(trackServerRequest), &(req.retval), sizeof(int));

	return req.retval;	
}
