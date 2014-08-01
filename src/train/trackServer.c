#include <train.h>




#define TRAIN_ACTUALDATA_SPEED					1

#define TRAIN_ACTUALDATA_STOP_DISTANCE			2
#define TRAIN_ACTUALDATA_STOP_TIME				3
#define TRAIN_ACTUALDATA_STOP_DECELERATION		4

#define TRAIN_ACTUALDATA_START_DISTANCE			5
#define TRAIN_ACTUALDATA_START_TIME				6
#define TRAIN_ACTUALDATA_START_ACCELERATION		7 


typedef struct trainStatus{
	int		isUsed;
	int 	trainNum;
	int 	trainWorkerTid;
	
	int 	previousTrainSpeed;
	int 	currentTrainSpeed;
	int		lastSpeedChange; 			//Time();
	
	int 	trainDirection;

	int 	lastTriggeredSensor;
	int 	lastTimeStemp;
	int 	currentDisplacement;
	int 	expectedSensor;
	int 	expectedSensorTime;
	int 	distToExpectedSensor;

	int 	delayOffset;

	//locationInfo 	destInfo;
}trainStatus;

typedef struct trackServerData{
	track_node 	trackA[TRACK_MAX];
	
	int 		trainsActualSpeeds[4][14];			//0->49, 1->50, 2->45, 3->48 in (mm/10ms * 10000)
	
	int			trainsStopDistances[4][14];			
	int 		trainsStopTimes[4][14];
	int 		trainsStopDeceleration[4][14];	

	int			trainsStartDistances[4][14];			
	int 		trainsStartTimes[4][14];
	int 		trainsStartAcceleration[4][14];	
	
	trainStatus trainsStatus[MAX_TRAINS];
	
	int			switchesStatus[MAX_SWITCHES];
	
	int 		initTrainNum;
}trackServerData;

static void
initTrainsSpecData(trackServerData* trkSvrData){
	trkSvrData->trainsActualSpeeds[1][0] = -1;		 	//train-54
	trkSvrData->trainsActualSpeeds[1][1] = -1; 
	trkSvrData->trainsActualSpeeds[1][2] = -1;
	trkSvrData->trainsActualSpeeds[1][3] = -1;
	trkSvrData->trainsActualSpeeds[1][4] = -1;
	trkSvrData->trainsActualSpeeds[1][5] = -1;
	trkSvrData->trainsActualSpeeds[1][6] = -1; 
	trkSvrData->trainsActualSpeeds[1][7] = 38028;
	trkSvrData->trainsActualSpeeds[1][8] = 42419; 
	trkSvrData->trainsActualSpeeds[1][9] = 47756; 
	trkSvrData->trainsActualSpeeds[1][10] = -1; 
	trkSvrData->trainsActualSpeeds[1][11] = -1; 
	trkSvrData->trainsActualSpeeds[1][12] = -1; 
	trkSvrData->trainsActualSpeeds[1][13] = -1; 

	trkSvrData->trainsActualSpeeds[2][0] = -1;		 	//train-45
	trkSvrData->trainsActualSpeeds[2][1] = -1; 
	trkSvrData->trainsActualSpeeds[2][2] = -1;
	trkSvrData->trainsActualSpeeds[2][3] = -1;
	trkSvrData->trainsActualSpeeds[2][4] = -1;
	trkSvrData->trainsActualSpeeds[2][5] = -1;
	trkSvrData->trainsActualSpeeds[2][6] = 33388; 
	trkSvrData->trainsActualSpeeds[2][7] = 37201;
	trkSvrData->trainsActualSpeeds[2][8] = 42301; 
	trkSvrData->trainsActualSpeeds[2][9] = 45633; 
	trkSvrData->trainsActualSpeeds[2][10] = 49947; 
	trkSvrData->trainsActualSpeeds[2][11] = 56606; 
	trkSvrData->trainsActualSpeeds[2][12] = -1; 
	trkSvrData->trainsActualSpeeds[2][13] = -1;  
	
	trkSvrData->trainsActualSpeeds[3][0] = -1;			//train-48 
	trkSvrData->trainsActualSpeeds[3][1] = -1; 
	trkSvrData->trainsActualSpeeds[3][2] = -1;
	trkSvrData->trainsActualSpeeds[3][3] = -1;
	trkSvrData->trainsActualSpeeds[3][4] = -1;
	trkSvrData->trainsActualSpeeds[3][5] = -1;
	trkSvrData->trainsActualSpeeds[3][6] = 33719; 
	trkSvrData->trainsActualSpeeds[3][7] = 37621;
	trkSvrData->trainsActualSpeeds[3][8] = 42515; 
	trkSvrData->trainsActualSpeeds[3][9] = 47268; 
	trkSvrData->trainsActualSpeeds[3][10] = 52609; 
	trkSvrData->trainsActualSpeeds[3][11] = 56982; 
	trkSvrData->trainsActualSpeeds[3][12] = -1; 
	trkSvrData->trainsActualSpeeds[3][13] = -1;     

	trkSvrData->trainsStopDistances[2][0] = -1; 		//train-45		
	trkSvrData->trainsStopDistances[2][1] = -1; 
	trkSvrData->trainsStopDistances[2][2] = -1; 
	trkSvrData->trainsStopDistances[2][3] = -1; 
	trkSvrData->trainsStopDistances[2][4] = -1; 
	trkSvrData->trainsStopDistances[2][5] = -1; 
	trkSvrData->trainsStopDistances[2][6] = 451; 
	trkSvrData->trainsStopDistances[2][7] = 495;
	trkSvrData->trainsStopDistances[2][8] = 570; 
	trkSvrData->trainsStopDistances[2][9] = 615; 
	trkSvrData->trainsStopDistances[2][10] = 710; 
	trkSvrData->trainsStopDistances[2][11] = 783; 
	trkSvrData->trainsStopDistances[2][12] = -1; 
	trkSvrData->trainsStopDistances[2][13] = -1;  
	
	trkSvrData->trainsStopDistances[3][0] =  -1; 		//train-48
	trkSvrData->trainsStopDistances[3][1] =  -1; 
	trkSvrData->trainsStopDistances[3][2] =  -1;
	trkSvrData->trainsStopDistances[3][3] =  -1; 
	trkSvrData->trainsStopDistances[3][4] =  -1; 
	trkSvrData->trainsStopDistances[3][5] =  -1; 
	trkSvrData->trainsStopDistances[3][6] =  498;
	trkSvrData->trainsStopDistances[3][7] =  564;
	trkSvrData->trainsStopDistances[3][8] =  626;
	trkSvrData->trainsStopDistances[3][9] =  709;
	trkSvrData->trainsStopDistances[3][10] = 790;
	trkSvrData->trainsStopDistances[3][11] = 873;
	trkSvrData->trainsStopDistances[3][12] = -1; 
	trkSvrData->trainsStopDistances[3][13] = -1;

	int i, j;
	for(i = 0; i < 4; i++){
		for(j = 0; j < 14; j++){
			int vi = trkSvrData->trainsActualSpeeds[i][j];
			int va = vi / 2;
			int stopDist = trkSvrData->trainsStopDistances[i][j];
			trkSvrData->trainsStopTimes[i][j] = stopDist * 10000 / va;
			trkSvrData->trainsStopDeceleration[i][j] = vi / trkSvrData->trainsStopTimes[i][j];
	
		}
	}
	
	trkSvrData->trainsStartDistances[2][0] = -1;		 	//train-45
	trkSvrData->trainsStartDistances[2][1] = -1; 
	trkSvrData->trainsStartDistances[2][2] = -1;
	trkSvrData->trainsStartDistances[2][3] = -1;
	trkSvrData->trainsStartDistances[2][4] = -1;
	trkSvrData->trainsStartDistances[2][5] = -1;
	trkSvrData->trainsStartDistances[2][6] = -1; 
	trkSvrData->trainsStartDistances[2][7] = -1;
	trkSvrData->trainsStartDistances[2][8] = -1; 
	trkSvrData->trainsStartDistances[2][9] = 912; 
	trkSvrData->trainsStartDistances[2][10] = -1; 
	trkSvrData->trainsStartDistances[2][11] = -1; 
	trkSvrData->trainsStartDistances[2][12] = -1; 
	trkSvrData->trainsStartDistances[2][13] = -1;  
	
	trkSvrData->trainsStartDistances[3][0] = -1;			//train-48 
	trkSvrData->trainsStartDistances[3][1] = -1; 
	trkSvrData->trainsStartDistances[3][2] = -1;
	trkSvrData->trainsStartDistances[3][3] = -1;
	trkSvrData->trainsStartDistances[3][4] = -1;
	trkSvrData->trainsStartDistances[3][5] = -1;
	trkSvrData->trainsStartDistances[3][6] = -1; 
	trkSvrData->trainsStartDistances[3][7] = -1;
	trkSvrData->trainsStartDistances[3][8] = -1; 
	trkSvrData->trainsStartDistances[3][9] = 930; 
	trkSvrData->trainsStartDistances[3][10] = -1; 
	trkSvrData->trainsStartDistances[3][11] = -1; 
	trkSvrData->trainsStartDistances[3][12] = -1; 
	trkSvrData->trainsStartDistances[3][13] = -1;  
	
	trkSvrData->trainsStartTimes[2][0] = -1;		 	//train-45
	trkSvrData->trainsStartTimes[2][1] = -1; 
	trkSvrData->trainsStartTimes[2][2] = -1;
	trkSvrData->trainsStartTimes[2][3] = -1;
	trkSvrData->trainsStartTimes[2][4] = -1;
	trkSvrData->trainsStartTimes[2][5] = -1;
	trkSvrData->trainsStartTimes[2][6] = -1; 
	trkSvrData->trainsStartTimes[2][7] = -1;
	trkSvrData->trainsStartTimes[2][8] = -1; 
	trkSvrData->trainsStartTimes[2][9] = 478; 
	trkSvrData->trainsStartTimes[2][10] = -1; 
	trkSvrData->trainsStartTimes[2][11] = -1; 
	trkSvrData->trainsStartTimes[2][12] = -1; 
	trkSvrData->trainsStartTimes[2][13] = -1;  
	
	trkSvrData->trainsStartTimes[3][0] = -1;			//train-48 
	trkSvrData->trainsStartTimes[3][1] = -1; 
	trkSvrData->trainsStartTimes[3][2] = -1;
	trkSvrData->trainsStartTimes[3][3] = -1;
	trkSvrData->trainsStartTimes[3][4] = -1;
	trkSvrData->trainsStartTimes[3][5] = -1;
	trkSvrData->trainsStartTimes[3][6] = -1; 
	trkSvrData->trainsStartTimes[3][7] = -1;
	trkSvrData->trainsStartTimes[3][8] = -1; 
	trkSvrData->trainsStartTimes[3][9] = 538; 
	trkSvrData->trainsStartTimes[3][10] = -1; 
	trkSvrData->trainsStartTimes[3][11] = -1; 
	trkSvrData->trainsStartTimes[3][12] = -1; 
	trkSvrData->trainsStartTimes[3][13] = -1; 	
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
		(trkSvrData->trainsStatus[i]).trainWorkerTid = -1; 
		
		(trkSvrData->trainsStatus[i]).previousTrainSpeed = 0;
		(trkSvrData->trainsStatus[i]).currentTrainSpeed  = 0;
		(trkSvrData->trainsStatus[i]).lastSpeedChange  = 0;

		(trkSvrData->trainsStatus[i]).trainDirection	= FORWARD;

		(trkSvrData->trainsStatus[i]).lastTriggeredSensor  = -1;
		(trkSvrData->trainsStatus[i]).lastTimeStemp  = -1;
		(trkSvrData->trainsStatus[i]).currentDisplacement = 0; 
		(trkSvrData->trainsStatus[i]).expectedSensor  = -1;
		(trkSvrData->trainsStatus[i]).expectedSensorTime  = -1;

		(trkSvrData->trainsStatus[i]).delayOffset = 0;
		
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

	trkSvrData->initTrainNum = 0;
}

static track_node*
getNextSensorNode(track_node* nextLandmark, int* totalDist, int* switchesStatus){
	
	while(nextLandmark->type != NODE_SENSOR && nextLandmark->type != NODE_EXIT){
		int direction = DIR_AHEAD;
		if(nextLandmark->type == NODE_BRANCH){
			int status = (nextLandmark->num <= 18) ? switchesStatus[(nextLandmark->num)-1] : 
														switchesStatus[(nextLandmark->num)-135] ;
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

static int
getTrainActualData(int whichData, trackServerData* trkSvrData, int trainNo, int trainSpeed){
	int whichRow;
		
	if(trainSpeed == 0)
		return 0;
	else{
		switch (trainNo)
		{
			case 54:
				whichRow = 1;
				break;
			case 45:
				whichRow = 2;
				break;
			case 48:
				whichRow = 3;
				break;		
		}
	}
	switch (whichData)
	{
		case TRAIN_ACTUALDATA_SPEED:
			return	trkSvrData->trainsActualSpeeds[whichRow][trainSpeed-1];	

		case TRAIN_ACTUALDATA_STOP_DISTANCE:
			return	trkSvrData->trainsStopDistances[whichRow][trainSpeed-1];	
		case TRAIN_ACTUALDATA_STOP_TIME:
			return	trkSvrData->trainsStopTimes[whichRow][trainSpeed-1];	
		case TRAIN_ACTUALDATA_STOP_DECELERATION:
			return	trkSvrData->trainsStopDeceleration[whichRow][trainSpeed-1];

		case TRAIN_ACTUALDATA_START_DISTANCE:
			return	trkSvrData->trainsStartDistances[whichRow][trainSpeed-1];	
		case TRAIN_ACTUALDATA_START_TIME:
			return	trkSvrData->trainsStartTimes[whichRow][trainSpeed-1];	
		case TRAIN_ACTUALDATA_START_ACCELERATION:
			return	trkSvrData->trainsStartAcceleration[whichRow][trainSpeed-1];				
	}			
}
static int
getTrainShortMoveTicks(int trainNo, int trainSpeed, int totalDist){
	//double x3 = totalDist * totalDist * totalDist;
	//double x2 = totalDist * totalDist;
	double x = totalDist;

	switch (trainNo)
	{
		case 45:
			switch (trainSpeed)
			{
				case 10:
					 return (0.236651 * x * x / 10000 * x / 100)
					 		- (0.647747 * x * x / 1000)
					 		+ (0.707508 * x) 
					 		+ 64.9667;
			}

			break;	
		case 48:
		{
			switch (trainSpeed)
			{
				case 10:
					 return (0.259327 * x * x / 10000 * x / 100)
					 		- (0.723554 * x * x / 1000)
					 		+ (0.776831 * x) 
					 		+ 66.5597;
			}

			break;
		}		
	}
}

static int
needToUpdate(trainStatus* trainsStatus, int sensor, int ts){
	int i;
	for (i = 0; i <MAX_TRAINS; i++)
	{
		if (trainsStatus[i].isUsed == 1 &&
				trainsStatus[i].lastTriggeredSensor == sensor && 
				ts - trainsStatus[i].lastTimeStemp <= 20)
			return 0;
	}
	
	return 1;
}

static void
trainWorker(){
	int trainNo;
	int trainSpeed;

	int requester;
	Receive(&requester, &trainNo, sizeof(int));
	Reply(requester, NULL, 0);
	
	trainWorkerRequest trainWorkerReq; 

	while(1){
		Receive(&requester, &trainWorkerReq, sizeof(trainWorkerRequest));
		Reply(requester, NULL, 0);
		
		trainSpeed = trainWorkerReq.trainSpeed;

		int i;
		for (i = 0; i < MAX_SWITCHES; i++)
		{
			if (trainWorkerReq.switchesChange[i] != -1){
				if (i < 18)
					changeSwitchStatus(i + 1, trainWorkerReq.switchesChange[i]);
				else
					changeSwitchStatus(i + 135, trainWorkerReq.switchesChange[i]);


				//sprintf(COM2, "%s\033[48;%uH%s switch id: %d %s", 
				//			save, clearLine, i*4, i, restore);		
			}		
		}
		
		changeTrainSpeed(trainNo,trainSpeed);

		int isPrevCommandReverse = 0;
		int x = 47;
		for (i = 0; i < trainWorkerReq.numberOfCommands; i++)
		{
			trainWorkerCommand* currentCommand = &(trainWorkerReq.commands[i]);
			
			switch (currentCommand->cmdType)
			{
				case REVERSE:
					reverseTrain(trainNo);
					isPrevCommandReverse = 1;

				break;
				case DELAY_STOP:
				{
					if(isPrevCommandReverse){
						changeTrainSpeed(trainNo,trainSpeed);
					}

					int totalDelay = currentCommand->cmdValue;

					sprintf(COM2, "%s\033[%u;50H totalDelay: %d%s", 
								save, x, totalDelay, restore);
					x++;

					
					while (1){
						totalDelay += getTrainDelayOffset(trainNo);

						//sprintf(COM2, "%s\033[46;0H totalDelay(modified):%d%s%s", 
						//	save, totalDelay, clearLine, restore);

						if (totalDelay > 45){
							Delay(40);
							totalDelay -= 40;
						}
						else
							break;
					}
					
					Delay(totalDelay);
					changeTrainSpeed(trainNo, 0);

					isPrevCommandReverse = 0;
				break;	
				}	
			}	
		}

		captureNodesByTrainNo(trainNo);

	}
	
	Exit();
}

void
trackServer(){
	trackServerData trkSvrData;
	
	int requester;
	trackServerRequest req;
	
	putc(COM1, 96);	
	initTrackServerData(&trkSvrData);
	
	int tempTrackIdx = TRACK_A_ENTRY_ONE; 
	
	while (1)
	{
		Receive(&requester, &req, sizeof(trackServerRequest));
		
		switch (req.trkSvrReq_uid)
		{
			case TRACKSERVER_INIT_TRAIN:
			{
				(trkSvrData.initTrainNum)++;
				
				trainStatus* thisTrainStat = &(trkSvrData.trainsStatus[req.target - 45]);		
				
				thisTrainStat->isUsed = 1;
				
				thisTrainStat->trainWorkerTid = Create(8, trainWorker);
				Send(thisTrainStat->trainWorkerTid, &(thisTrainStat->trainNum), sizeof(int), NULL, 0);
				
				switch (req.value)
				{
					case TRACK_A_ENTRY_ONE :
						thisTrainStat->lastTriggeredSensor	= 16 + 9 - 1;	//B9
						thisTrainStat->lastTimeStemp 		= req.ts;	
						thisTrainStat->currentDisplacement  = 60;

						thisTrainStat->expectedSensor 		= 5 - 1;  		//A5
						
						break;

					case TRACK_A_ENTRY_TWO :
						thisTrainStat->lastTriggeredSensor	= 16 + 11 - 1;	//B11
						thisTrainStat->lastTimeStemp 		= req.ts;	
						thisTrainStat->currentDisplacement  = 60;
						
						thisTrainStat->expectedSensor 		= 8 - 1; 		//A8	
					
						break;

					case TRACK_A_ENTRY_THREE :
						thisTrainStat->lastTriggeredSensor	= 16 + 7 - 1;		//B7
						thisTrainStat->lastTimeStemp 		= req.ts;
						thisTrainStat->currentDisplacement  = 60;	
						
						thisTrainStat->expectedSensor 		= 10 - 1; 			//A10	
					
						break;	
				}
				
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

				//TODO: multi train
				if (req.target == 45){
					sprintf(COM2, "%s\033[5;65H%s%d%s", save, clearLine, req.value % 15, restore);
				}
				else if (req.target == 48){
					sprintf(COM2, "%s\033[6;65H%s%d%s", save, clearLine, req.value % 15, restore);
				}
				else{
					sprintf(COM2, "%s\033[7;65H%s%d%s", save, clearLine, req.value % 15, restore);
				}
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
					//sprintf(COM2, "%s\033[4;70H%sBackward%s", save, clearLine, restore);
				}
				else{
					trkSvrData.trainsStatus[req.target-45].trainDirection = FORWARD;
					//sprintf(COM2, "%s\033[4;70H%sForward%s", save, clearLine, restore);
				}
				
				Reply(requester, NULL, 0);
				break;
			}
			case TRACKSERVER_UPDATE_LAST_SENSOR:
			{
				if (trkSvrData.initTrainNum > 0){
					/*
					 * Init: handle trains' direction(reverse case)
					 */									
					if(	req.value == 	(16 + 10 - 1) 	//B10
						|| req.value == (16 + 12 - 1) 	//B12
						|| req.value == (16 +  8 - 1) 	//B8
					){	
						int i;
						for (i = 0; i <MAX_TRAINS; i++)
						{
							if ((trkSvrData.trainsStatus[i].lastTriggeredSensor + 1) == req.value)
								break;
						}
						
						trainStatus* thisTrainStat = &(trkSvrData.trainsStatus[i]);		
						
						putc(COM1, 0);
						putc(COM1, thisTrainStat->trainNum);					
						Delay(100);																										
						putc(COM1, 15);
						putc(COM1, thisTrainStat->trainNum);						
									
						putc(COM1, 10);
						putc(COM1, thisTrainStat->trainNum);
						Delay(150);							
						putc(COM1, 0);
						putc(COM1, thisTrainStat->trainNum);	
						
						//thisTrainStat->currentDisplacement  = -30;								
						
						(trkSvrData.initTrainNum)--;
							
						(req.value)--;
					}
					else
						(trkSvrData.initTrainNum)--;
				}
				if (needToUpdate(trkSvrData.trainsStatus, req.value, req.ts) == 1){	
					/*
					 * Previous sensor(15;0HPrevious sensor:)
					 * Actual time(16;4HActual time:)
					 */
					// int sensorGroup = req.value / 16 + 'A';
					// int sensorId = req.value % 16 + 1;
					
					// int ds = req.ts / 10 % 10;
					// int s = req.ts / 100 % 60;
					// int m = req.ts / 6000 % 60;
					// int h = req.ts / 360000;
					// sprintf(COM2, "%s\033[15;22H%s%s%c%d\033[16;22H%s%d:%d:%d:%d%s%s", 
					// 	save, yellow, clearLine, (char)sensorGroup, sensorId, clearLine, h, m, s, ds, resetColor, restore);

					int sensorIndex = req.value;
					int trainIdx = determineTrainByTriggeredSensor(sensorIndex, trkSvrData.trainsStatus);	//expected sensor handling(normal case)
									
					if (trainIdx == -1){																	//expected sensor handling(special case)
						for (trainIdx = 0; trainIdx <MAX_TRAINS; trainIdx++)
						{
							if (trkSvrData.trainsStatus[trainIdx].isUsed == 1){
								int expectedSensor = trkSvrData.trainsStatus[trainIdx].expectedSensor;
								int dist, nextSensor;

								track_node* nextSensorNode;

								/*
								 * nextNode of expectedSensor
								 */ 								
								track_node*	aheadExpectedSensorNode = trkSvrData.trackA[expectedSensor].edge[DIR_AHEAD].dest;
															   dist = trkSvrData.trackA[expectedSensor].edge[DIR_AHEAD].dist;
													
								nextSensorNode = getNextSensorNode(aheadExpectedSensorNode, &dist, trkSvrData.switchesStatus);
								
								nextSensor = nextSensorNode->num;

								if (req.value == nextSensor){
									trainStatus *trainStat = &(trkSvrData.trainsStatus[trainIdx]);
									int   trainActualSpeed = getTrainActualData(TRAIN_ACTUALDATA_SPEED, 
														&trkSvrData, trainStat->trainNum, trainStat->currentTrainSpeed);	

									trainStat->expectedSensorTime += (dist * 10000 / trainActualSpeed);
									break;
								}
							}						
						}	
					}
					
					if (trainIdx == MAX_TRAINS || trainIdx < 0){
						//sprintf(COM2, "%s\033[45;0H%s Can't distribute sensor %c%d, drop this sensor! %s", 
						//					save, clearLine, (char)sensorGroup, sensorId, restore);
					}
					else{										
						trainStatus *trainStat = &(trkSvrData.trainsStatus[trainIdx]);
						
						releaseNodesBySensor(trainStat->trainNum, sensorIndex);
							
						/*
						 * Expected time(17;4HExpected time:)
						 */
						// int expt = trainStat->expectedSensorTime + trainStat->lastTimeStemp;
						// ds = expt / 10 % 10;
						// s = expt / 100 % 60;
						// m = expt / 6000 % 60;
						// h = expt / 360000;
						// sprintf(COM2, "%s\033[17;22H%s%s%d:%d:%d:%d%s%s", 
						// 	save, yellow, clearLine, h, m, s, ds, resetColor, restore);
						/*
						 * Time difference(18;4HTime difference:)
						 */				 
						// int diff = expt - req.ts;

						// if (diff > -30 && diff < 30){
						// 	trainStat->delayOffset = trainStat->delayOffset - diff;
						// }

						// sprintf(COM2, "%s\033[18;22H%s%s%d ticks%s%s", 
						// 	save, yellow, clearLine, diff, resetColor, restore);
						
						
						track_node*	nextLandmark 	= trkSvrData.trackA[sensorIndex].edge[DIR_AHEAD].dest;
						int 		totalDist 		= trkSvrData.trackA[sensorIndex].edge[DIR_AHEAD].dist;

						track_node* nextSensorNode = getNextSensorNode(nextLandmark, &totalDist, trkSvrData.switchesStatus);					

						int trainActualSpeed = getTrainActualData(TRAIN_ACTUALDATA_SPEED, 
														&trkSvrData, trainStat->trainNum, trainStat->currentTrainSpeed);		

						trainStat->lastTriggeredSensor 	= sensorIndex;
						trainStat->lastTimeStemp		= req.ts;
						trainStat->expectedSensor 		= nextSensorNode->num; //((nextSensorNode->num / 16) + 'A') * 17 + ((nextSensorNode->num % 16) + 1); 
						trainStat->distToExpectedSensor = totalDist;
						int t = (totalDist * 10000 / trainActualSpeed);			// * trkSvrData.trackFraction[nextLandmark->num-1] ; // ticks
						trainStat->expectedSensorTime = t;
						
						t = t + req.ts;

						// ds = t / 10 % 10;
						// s = t / 100 % 60;
						// m = t / 6000 % 60;
						// h = t / 360000;
						// sprintf(COM2, "%s%s\033[13;16H%s%s\033[13;21Hat: %d:%d:%d:%d%s", 
						// 	save, yellow, clearLine, nextSensorNode->name, h, m, s, ds, restore);			
					}
				}
				
				Reply(requester, NULL, 0);
				break;
			}
			case TRACKSERVER_TRAIN_DELAYOFFSET_GET:
			{
				trainStatus *trainStat = &(trkSvrData.trainsStatus[req.target-45]);	
				int delayOffset = trainStat->delayOffset;

				trainStat->delayOffset = 0;

				Reply(requester, &(delayOffset), sizeof(int));								
			}
			case TRACKSERVER_TRAIN_LOCATION_GET:
			{	
				trainStatus *trainStat = &(trkSvrData.trainsStatus[req.target-45]);
				int 		 trainNum = req.target;
				int 		 trainSpeed = trainStat->currentTrainSpeed;
				int 		 prevTrainSpeed = trainStat->previousTrainSpeed;
				int 		 speedChangeTime = trainStat->lastSpeedChange;

				int	curVelocity = getTrainActualData(TRAIN_ACTUALDATA_SPEED, &trkSvrData, trainNum, trainSpeed);
				int	prevVelocity = getTrainActualData(TRAIN_ACTUALDATA_SPEED, &trkSvrData, trainNum, prevTrainSpeed);

				//int i = trkSvrData.currentTrain; 	//for demo 1
				int sensorTimeStemp = trainStat->lastTimeStemp; //trkSvrData.trainsStatus[i].lastTimeStemp;
				locationInfo locInfo;
				int displacement;

				if (trainSpeed == 0 && prevTrainSpeed == 0){
					locInfo.displacement = trainStat->currentDisplacement;
					locInfo.sensor = trainStat->lastTriggeredSensor;
					
					Reply(requester, &locInfo, sizeof(locationInfo));
					break;
				}

				if (trainSpeed > 0){
					if (prevTrainSpeed == 0 && speedChangeTime > sensorTimeStemp){
						displacement = (Time() - speedChangeTime) * curVelocity / 10000 + trainStat->currentDisplacement;
					}
					// else if (speedChangeTime > sensorTimeStemp){
					// 	displacement = (Time() - speedChangeTime) * curVelocity / 10000 
					// 					+ (speedChangeTime - sensorTimeStemp) * prevVelocity / 10000;
					// }
					else{
						displacement = (Time() - sensorTimeStemp) * curVelocity / 10000;
					}
				}
				else{
					curVelocity = prevVelocity / 2;
					int stopDist = 	getTrainActualData(TRAIN_ACTUALDATA_STOP_DISTANCE, &trkSvrData, trainNum, prevTrainSpeed);
					int stopTime = 	getTrainActualData(TRAIN_ACTUALDATA_STOP_TIME, &trkSvrData, trainNum, prevTrainSpeed);
					int deceleration = 	getTrainActualData(TRAIN_ACTUALDATA_STOP_DECELERATION, &trkSvrData, trainNum, prevTrainSpeed);

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

				//displacement += (trainStat->trainDirection == FORWARD) ? 140 : 20;
				displacement -= (trainStat->trainDirection == FORWARD) ? 20 : 140;
				trainStat->currentDisplacement = displacement;


				locInfo.displacement = displacement;
				locInfo.sensor = trainStat->lastTriggeredSensor;
				
				Reply(requester, &locInfo, sizeof(locationInfo));
				break;
			}
			case TRACKSERVER_EXECUTE_PATH:
			{
				
				trainStatus* 	thisTrainStatus	= &(trkSvrData.trainsStatus[req.target-45]);
				trainPath*		thisTrainPath 	= req.path;

				int trainNo 	= thisTrainStatus->trainNum;
				int trainSpeed 	= thisTrainPath->trainSpeed;
				
				/*
				 * 4. trackServer parse this command and formated this path	
				 */ 
				 
				if(thisTrainPath->path[0] != -1){		
					int i;
					int a = 0;	
		
					for (i = thisTrainPath->path[0]; i < TRACK_MAX; i++)
					{
							//sprintf(COM2, "%s\033[45;%uH%s%s%s", 
							//	save, a, clearLine, trkSvrData.trackA[(thisTrainPath->path[i])].name, restore);
							//a = a + 6;					
					}

					trainWorkerRequest req;
					/*
					 * Init switches
					 */ 
					for (i = 0; i < MAX_SWITCHES; i++)
					{
						req.switchesChange[i] = -1;
					}
					
					req.trainSpeed = trainSpeed;

					req.numberOfCommands = 0;
					
					int src  = thisTrainPath->path[thisTrainPath->path[0]];
					int dest = thisTrainPath->path[TRACK_MAX-1];
					int destNode = &(trkSvrData.trackA[dest]);
					
					int curvesNum = 0;				 
					int currentDist = 0 - thisTrainStatus->currentDisplacement;


					int actualSpeed = getTrainActualData(TRAIN_ACTUALDATA_SPEED, &trkSvrData, trainNo, trainSpeed);

					int stopDist  = getTrainActualData(TRAIN_ACTUALDATA_STOP_DISTANCE, &trkSvrData, trainNo, trainSpeed);
					int startDist = getTrainActualData(TRAIN_ACTUALDATA_START_DISTANCE, &trkSvrData, trainNo, trainSpeed);

					int startTime = getTrainActualData(TRAIN_ACTUALDATA_START_TIME, &trkSvrData, trainNo, trainSpeed);
				 
					int x = 47;

					for (i = thisTrainPath->path[0]; i < TRACK_MAX; i++)
					{
							track_node* thisNode = &(trkSvrData.trackA[thisTrainPath->path[i]]);
							//track_node* reverseNode = thisNode->reverse;	//reverseNode of thisNode

							track_node* nextNode = &(trkSvrData.trackA[thisTrainPath->path[i+1]]);
						
							switch (thisNode->type)
							{
								case NODE_SENSOR:
								case NODE_MERGE:
								{
									if (thisNode->reverse != nextNode){
										currentDist += thisNode->edge[DIR_AHEAD].dist;
									}
									
									break;
								}
								case NODE_BRANCH:
								{
									if (thisNode->edge[DIR_STRAIGHT].dest == nextNode){
										if(thisNode->num <= 18)
											req.switchesChange[thisNode->num - 1] = STRAIGHT;
										else
											req.switchesChange[thisNode->num - 135] = STRAIGHT;
											
										currentDist += thisNode->edge[DIR_STRAIGHT].dist;
									}
									else if (thisNode->edge[DIR_CURVED].dest == nextNode){
										if(thisNode->num <= 18)
											req.switchesChange[thisNode->num - 1] = CURVED;
										else
											req.switchesChange[thisNode->num - 135] = CURVED;
										
										currentDist += thisNode->edge[DIR_CURVED].dist;
										curvesNum++;
									}	
								}
							}

							//TODO: if (nextNode == thisNode->reverse)
							if(nextNode == thisNode->reverse){
								int totalDist, totalDelay;
								if (nextNode == destNode){

									if (currentDist > startDist + stopDist){
										totalDist  = currentDist - stopDist - startDist;	
										totalDelay = (totalDist * 10000 / actualSpeed) + startTime;// + curvesNum * 10;

										sprintf(COM2, "%s\033[%u;0H %s totalDist:%d totalDelay:%dcurvesNum:%d %s", 
											save, x, green, totalDist, totalDelay, curvesNum,restore);
										x++;
									}
									else{
										totalDist  = currentDist;
										totalDelay = getTrainShortMoveTicks(trainNo, trainSpeed, totalDist);

										sprintf(COM2, "%s\033[%u;0H %s totalDist:%d totalDelay:%dcurvesNum:%d %s", 
											save, x, yellow, totalDist, totalDelay, curvesNum,restore);
										x++;
									}
										
									req.commands[req.numberOfCommands] = (trainWorkerCommand){DELAY_STOP, totalDelay};
									req.numberOfCommands++;	

									req.commands[req.numberOfCommands] = (trainWorkerCommand){REVERSE, -1};
									req.numberOfCommands++;	

									req.commands[req.numberOfCommands] = (trainWorkerCommand){DELAY_STOP, getTrainShortMoveTicks(trainNo, trainSpeed, 200)};
									req.numberOfCommands++;	
									break;
								}
								else{

									currentDist += 300;
									if (currentDist > startDist + stopDist){
										totalDist  = currentDist - stopDist - startDist;	
										totalDelay = (totalDist * 10000 / actualSpeed) + startTime;// + curvesNum * 10;

										sprintf(COM2, "%s\033[%u;0H %s totalDist:%d totalDelay:%dcurvesNum:%d %s", 
											save, x, green, totalDist, totalDelay, curvesNum,restore);
										x++;
									}
									else{
										totalDist  = currentDist;
										totalDelay = getTrainShortMoveTicks(trainNo, trainSpeed, totalDist);

										sprintf(COM2, "%s\033[%u;0H %s totalDist:%d totalDelay:%dcurvesNum:%d %s", 
											save, x, yellow, totalDist, totalDelay, curvesNum,restore);
										x++;
									}
										
									req.commands[req.numberOfCommands] = (trainWorkerCommand){DELAY_STOP, totalDelay};
									req.numberOfCommands++;	

									req.commands[req.numberOfCommands] = (trainWorkerCommand){REVERSE, -1};
									req.numberOfCommands++;	

									currentDist = 500;//200;
								}
							}
							else if (nextNode == destNode){
								int totalDist, totalDelay;
								if (currentDist > startDist + stopDist){
									totalDist  = currentDist - stopDist - startDist;	
									totalDelay = (totalDist * 10000 / actualSpeed) + startTime;// + curvesNum * 10;
								}
								else{
									totalDist  = currentDist;
									totalDelay = getTrainShortMoveTicks(trainNo, trainSpeed, totalDist);
								}

								sprintf(COM2, "%s\033[50;0H totalDist:%d totalDelay:%d curvesNum:%d %s", 
									save, totalDist, totalDelay, curvesNum, restore);
									
								req.commands[req.numberOfCommands] = (trainWorkerCommand){DELAY_STOP, totalDelay};
								req.numberOfCommands++;	
								
								break;						
							}		
					}
					
					/*
					 * 5. trackServer pass this formated path to the trainCommandWorker	
					 */
					Send(thisTrainStatus->trainWorkerTid, &(req), sizeof(trainWorkerRequest), NULL, 0); 
					break;	 									
				}
				else{
					//errorMsg;
				}				
				
				Reply(requester, NULL, 0);
				
				break;
			}
			case TRACKSERVER_DIST_BETWEEN_SENSORS:
			{

				int sensor1 = req.value;
				int sensor2 = req.value2;

				int dist = trkSvrData.trackA[sensor1].edge[DIR_AHEAD].dist;		
				track_node*	temp = trkSvrData.trackA[sensor1].edge[DIR_AHEAD].dest;		
				temp = getNextSensorNode(temp, &dist, trkSvrData.switchesStatus);
				int nextSensorNum = temp->num;
				int i = 1;
				while (nextSensorNum != sensor2){
					dist += temp->edge[DIR_AHEAD].dist;	
					temp = temp->edge[DIR_AHEAD].dest;
					temp = getNextSensorNode(temp, &dist, trkSvrData.switchesStatus);
					nextSensorNum = temp->num;
					i++;
					if (temp->type != NODE_SENSOR  || i > 10){
						dist = -1;
						break;
					}
				}			

				// sprintf(COM2, "%s\033[25;32H%s%s to %s = %dmm%s", 
		 	// 		save, clearLine, 
		 	// 		trkSvrData.trackA[sensor1].name, 
		 	// 		trkSvrData.trackA[sensor2].name, 
		 	// 		dist, restore);									

				Reply(requester, &dist, sizeof(int));
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

	Delay(500);									//delay 2s

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
initTrain(int trainNo, int entryPoint){
	putc(COM1, 10);
	putc(COM1, trainNo);
	
	trackServerRequest req;
	
	req.trkSvrReq_uid = TRACKSERVER_INIT_TRAIN;	
	req.target 	= trainNo;
	req.value 	= entryPoint;

	Send(TRACKSERVER_TID, &req, sizeof(trackServerRequest), NULL, 0);
	
	Delay(100);
	
	putc(COM1, 0);
	putc(COM1, trainNo);
}

int
getTrainDelayOffset(int trainNo){
	trackServerRequest req;
		
	req.trkSvrReq_uid = TRACKSERVER_TRAIN_DELAYOFFSET_GET;	
	req.target = trainNo;
	
	Send(TRACKSERVER_TID, &req, sizeof(trackServerRequest), &(req.retval), sizeof(int));	
	
	return req.retval;	
}

locationInfo
getTrainLocation(int trainNo){
	locationInfo response;
	trackServerRequest req;
	
	req.trkSvrReq_uid = TRACKSERVER_TRAIN_LOCATION_GET;
		
	req.target 	= trainNo;

	Send(TRACKSERVER_TID, &req, sizeof(trackServerRequest), &response, sizeof(locationInfo));

	int temp = response.sensor;
	response.sensor = ((temp / 16) + 'A') * 17 + (temp % 16 + 1);

	return response;
}

void
executePath(int trainNo, trainPath* path){
	trackServerRequest req;
		
	req.trkSvrReq_uid = TRACKSERVER_EXECUTE_PATH;
		
	req.target 	= trainNo;
	req.path	= path;
	
	Send(TRACKSERVER_TID, &req, sizeof(trackServerRequest), NULL, 0);	
}

int
getDistBetweenSensors(int sensor1, int sensor2){
	int response;
	trackServerRequest req;
	
	req.trkSvrReq_uid = TRACKSERVER_DIST_BETWEEN_SENSORS;
		
	req.value = (sensor1 % 17 + (sensor1 / 17 - 'A') * 16) - 1;
	req.value2 = (sensor2 % 17 + (sensor2 / 17 - 'A') * 16) - 1;

	Send(TRACKSERVER_TID, &req, sizeof(trackServerRequest), &response, sizeof(locationInfo));

	return response;
}