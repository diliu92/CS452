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
	int 		currentTrain;
}trackServerData;

static void
initTrainsSpecData(trackServerData* trkSvrData){	

	trkSvrData->trainsActualSpeeds[0][0] = 1348;		//train-49
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
	
	trkSvrData->trainsActualSpeeds[1][0] =  945; 		//train-50
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

	trkSvrData->trainsStopDistances[0][0] = -1; 		//train-49 	
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
	
	trkSvrData->trainsStopDistances[1][0] =  -1; 		//train-50
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

	trkSvrData->trainsStartDistances[0][0] = -1;		//train-49
	trkSvrData->trainsStartDistances[0][1] = -1; 
	trkSvrData->trainsStartDistances[0][2] = -1;
	trkSvrData->trainsStartDistances[0][3] = -1;
	trkSvrData->trainsStartDistances[0][4] = -1;
	trkSvrData->trainsStartDistances[0][5] = -1;
	trkSvrData->trainsStartDistances[0][6] = 425;//436; 
	trkSvrData->trainsStartDistances[0][7] = 559;//562;
	trkSvrData->trainsStartDistances[0][8] = 651;//641; 
	trkSvrData->trainsStartDistances[0][9] = 737;//710; 
	trkSvrData->trainsStartDistances[0][10] = 950;//895; 
	trkSvrData->trainsStartDistances[0][11] = 1014;//935; 
	trkSvrData->trainsStartDistances[0][12] = -1; 
	trkSvrData->trainsStartDistances[0][13] = -1;  
	
	trkSvrData->trainsStartDistances[1][0] =  -1; 		//train-50
	trkSvrData->trainsStartDistances[1][1] =  -1; 
	trkSvrData->trainsStartDistances[1][2] =  -1;
	trkSvrData->trainsStartDistances[1][3] =  -1; 
	trkSvrData->trainsStartDistances[1][4] =  -1; 
	trkSvrData->trainsStartDistances[1][5] =  -1; 
	trkSvrData->trainsStartDistances[1][6] =  -1; 
	trkSvrData->trainsStartDistances[1][7] =  -1; 	
	trkSvrData->trainsStartDistances[1][8] =  -1; 	
	trkSvrData->trainsStartDistances[1][9] =  -1; 	
	trkSvrData->trainsStartDistances[1][10] = -1; 	
	trkSvrData->trainsStartDistances[1][11] = -1; 	
	trkSvrData->trainsStartDistances[1][12] = -1; 	
	trkSvrData->trainsStartDistances[1][13] = -1;
	
	trkSvrData->trainsStartDistances[2][0] = -1;		 	//train-45
	trkSvrData->trainsStartDistances[2][1] = -1; 
	trkSvrData->trainsStartDistances[2][2] = -1;
	trkSvrData->trainsStartDistances[2][3] = -1;
	trkSvrData->trainsStartDistances[2][4] = -1;
	trkSvrData->trainsStartDistances[2][5] = -1;
	trkSvrData->trainsStartDistances[2][6] = 33388; 
	trkSvrData->trainsStartDistances[2][7] = 37201;
	trkSvrData->trainsStartDistances[2][8] = 42301; 
	trkSvrData->trainsStartDistances[2][9] = 45633; 
	trkSvrData->trainsStartDistances[2][10] = 49947; 
	trkSvrData->trainsStartDistances[2][11] = 56606; 
	trkSvrData->trainsStartDistances[2][12] = -1; 
	trkSvrData->trainsStartDistances[2][13] = -1;  
	
	trkSvrData->trainsStartDistances[3][0] = -1;			//train-48 
	trkSvrData->trainsStartDistances[3][1] = -1; 
	trkSvrData->trainsStartDistances[3][2] = -1;
	trkSvrData->trainsStartDistances[3][3] = -1;
	trkSvrData->trainsStartDistances[3][4] = -1;
	trkSvrData->trainsStartDistances[3][5] = -1;
	trkSvrData->trainsStartDistances[3][6] = 33719; 
	trkSvrData->trainsStartDistances[3][7] = 37621;
	trkSvrData->trainsStartDistances[3][8] = 42515; 
	trkSvrData->trainsStartDistances[3][9] = 47268; 
	trkSvrData->trainsStartDistances[3][10] = 52609; 
	trkSvrData->trainsStartDistances[3][11] = 56982; 
	trkSvrData->trainsStartDistances[3][12] = -1; 
	trkSvrData->trainsStartDistances[3][13] = -1;  


	trkSvrData->trainsStartTimes[0][0] = -1;		//train-49
	trkSvrData->trainsStartTimes[0][1] = -1; 
	trkSvrData->trainsStartTimes[0][2] = -1;
	trkSvrData->trainsStartTimes[0][3] = -1;
	trkSvrData->trainsStartTimes[0][4] = -1;
	trkSvrData->trainsStartTimes[0][5] = -1;
	trkSvrData->trainsStartTimes[0][6] = 349; //352 
	trkSvrData->trainsStartTimes[0][7] = 382; //383; 
	trkSvrData->trainsStartTimes[0][8] = 402; //400; 
	trkSvrData->trainsStartTimes[0][9] = 419; //414; 
	trkSvrData->trainsStartTimes[0][10] = 456;//447; 
	trkSvrData->trainsStartTimes[0][11] = 466;//453; 
	trkSvrData->trainsStartTimes[0][12] = -1; 
	trkSvrData->trainsStartTimes[0][13] = -1;  
	
	trkSvrData->trainsStartTimes[1][0] =  -1; 		//train-50
	trkSvrData->trainsStartTimes[1][1] =  -1; 
	trkSvrData->trainsStartTimes[1][2] =  -1;
	trkSvrData->trainsStartTimes[1][3] =  -1; 
	trkSvrData->trainsStartTimes[1][4] =  -1; 
	trkSvrData->trainsStartTimes[1][5] =  -1; 
	trkSvrData->trainsStartTimes[1][6] =  -1; 
	trkSvrData->trainsStartTimes[1][7] =  -1; 	
	trkSvrData->trainsStartTimes[1][8] =  -1; 	
	trkSvrData->trainsStartTimes[1][9] =  -1; 	
	trkSvrData->trainsStartTimes[1][10] = -1; 	
	trkSvrData->trainsStartTimes[1][11] = -1; 	
	trkSvrData->trainsStartTimes[1][12] = -1; 	
	trkSvrData->trainsStartTimes[1][13] = -1;
	
	trkSvrData->trainsStartTimes[2][0] = -1;		 	//train-45
	trkSvrData->trainsStartTimes[2][1] = -1; 
	trkSvrData->trainsStartTimes[2][2] = -1;
	trkSvrData->trainsStartTimes[2][3] = -1;
	trkSvrData->trainsStartTimes[2][4] = -1;
	trkSvrData->trainsStartTimes[2][5] = -1;
	trkSvrData->trainsStartTimes[2][6] = 33388; 
	trkSvrData->trainsStartTimes[2][7] = 37201;
	trkSvrData->trainsStartTimes[2][8] = 42301; 
	trkSvrData->trainsStartTimes[2][9] = 45633; 
	trkSvrData->trainsStartTimes[2][10] = 49947; 
	trkSvrData->trainsStartTimes[2][11] = 56606; 
	trkSvrData->trainsStartTimes[2][12] = -1; 
	trkSvrData->trainsStartTimes[2][13] = -1;  
	
	trkSvrData->trainsStartTimes[3][0] = -1;			//train-48 
	trkSvrData->trainsStartTimes[3][1] = -1; 
	trkSvrData->trainsStartTimes[3][2] = -1;
	trkSvrData->trainsStartTimes[3][3] = -1;
	trkSvrData->trainsStartTimes[3][4] = -1;
	trkSvrData->trainsStartTimes[3][5] = -1;
	trkSvrData->trainsStartTimes[3][6] = 33719; 
	trkSvrData->trainsStartTimes[3][7] = 37621;
	trkSvrData->trainsStartTimes[3][8] = 42515; 
	trkSvrData->trainsStartTimes[3][9] = 47268; 
	trkSvrData->trainsStartTimes[3][10] = 52609; 
	trkSvrData->trainsStartTimes[3][11] = 56982; 
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

		//(trkSvrData->trainsStatus[i]).destInfo.sensor = 0;
		//(trkSvrData->trainsStatus[i]).destInfo.displacement = 0;
		
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
			case 49:
				whichRow = 0;
				break;
			case 50:
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
	int x3 = totalDist * totalDist * totalDist;
	int x2 = totalDist * totalDist;
	int x = totalDist;

	switch (trainNo)
	{
		case 49:
		{
			switch (trainSpeed)
			{
				case 7:
					return 2 * x3 / 10000000 - 5 * x2 / 10000 + 636 * x / 1000 + 63625 / 1000;
				case 8:
					return 3 * x3 / 10000000 - 7 * x2 / 10000 + 708 * x / 1000 + 54316 / 1000;
				case 9:
					return 2 * x3 / 10000000 - 6 * x2 / 10000 + 676 * x / 1000 + 56221 / 1000;
				case 10:
					return 1 * x3 / 10000000 - 4 * x2 / 10000 + 584 * x / 1000 + 62767 / 1000;
				case 11:
					return 1 * x3 / 10000000 - 4 * x2 / 10000 + 576 * x / 1000 + 63421 / 1000;
			}

			break;
		}
		case 50:
			break;	
		case 45:
			break;	
		case 48:
		{
			switch (trainSpeed)
			{
				case 7:
					return 2 * x3 / 10000000 - 5 * x2 / 10000 + 636 * x / 1000 + 63625 / 1000;
				case 8:
					return 3 * x3 / 10000000 - 7 * x2 / 10000 + 708 * x / 1000 + 54316 / 1000;
				case 9:
					return 2 * x3 / 10000000 - 6 * x2 / 10000 + 676 * x / 1000 + 56221 / 1000;
				case 10:
					return 1 * x3 / 10000000 - 4 * x2 / 10000 + 584 * x / 1000 + 62767 / 1000;
				case 11:
					return 1 * x3 / 10000000 - 4 * x2 / 10000 + 576 * x / 1000 + 63421 / 1000;
			}

			break;
		}		
	}

}



static void
trainWorker(){
	int trainNo;
	int trainSpeed;

	int requester;
	Receive(&requester, &trainNo, sizeof(int));
	Reply(requester, NULL, 0);
	
	trainWorkerRequest trainWorkerReq; 

	while(!NeedToShutDown()){
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
		for (i = 0; i < trainWorkerReq.numberOfCommands; i++)
		{
			trainWorkerCommand* currentCommand = &(trainWorkerReq.commands[i]);
			
			switch (currentCommand->cmdType)
			{
				//case RUN:
					//changeTrainSpeed(trainNo, trainSpeed);
				//break;
				case REVERSE:
					reverseTrain(trainNo);
					//changeTrainSpeed(trainNo,trainSpeed);

					//Delay(currentCommand->cmdValue);

					//changeTrainSpeed(trainNo, 0);
					//changeTrainSpeed(trainNo, currentCommand->cmdValue);
					isPrevCommandReverse = 1;

				break;
				case DELAY_STOP:
				{
					if(isPrevCommandReverse){
						changeTrainSpeed(trainNo,trainSpeed);
					}

					int totalDelay = currentCommand->cmdValue;

					while (1){
						totalDelay += getTrainDelayOffset(trainNo);

						//sprintf(COM2, "%s\033[46;0H totalDelay(modified):%d%s%s", 
						//	save, totalDelay, clearLine, restore);

						if (totalDelay > 40){
							Delay(30);
							totalDelay -= 30;
						}
						else
							break;
					}
					Delay(totalDelay);
					changeTrainSpeed(trainNo, 0);

					isPrevCommandReverse = 0;
				}
				break;		
			}	
		}


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
				trkSvrData.initTrainNum = req.target;
				trkSvrData.currentTrain = req.target - 45;
				
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
					/*
					 * Init: handle trains' direction(reverse case)
					 */ 
					trainStatus* thisTrainStat = &(trkSvrData.trainsStatus[trkSvrData.initTrainNum - 45]);		
					
					if(	req.value == 	(16 + 10 - 1) 	//B10
						|| req.value == (16 + 12 - 1) 	//B12
						|| req.value == (16 +  8 - 1) 	//B8
					){											
						thisTrainStat->currentDisplacement  = -30;
									
						putc(COM1, 0);
						putc(COM1, trkSvrData.initTrainNum);

						putc(COM1, 15);
						putc(COM1, trkSvrData.initTrainNum);
						
						putc(COM1, 0);
						putc(COM1, trkSvrData.initTrainNum);
					}

					trkSvrData.initTrainNum = -1;
				}
				else {	
					/*
					 * Previous sensor(15;0HPrevious sensor:)
					 * Actual time(16;4HActual time:)
					 */
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

								/*
								 * reverseNode of lastTriggeredSensor
								 */ 
								int lastTriggeredSensor = trkSvrData.trainsStatus[trainIdx].lastTriggeredSensor;
								track_node*	reverseLastTriggeredSensorNode = trkSvrData.trackA[lastTriggeredSensor].reverse;
										
								int reverseLastTriggeredSensor = reverseLastTriggeredSensorNode->num;
								
								if (req.value == reverseLastTriggeredSensor){
									//trainStatus *trainStat = &(trkSvrData.trainsStatus[trainIdx]);

									break;
								}


								/*
								 * ahead of reverse of expectedSensor
								 */ 
								track_node* aheadOfReverseOfExpectedSensor = trkSvrData.trackA[expectedSensor].reverse->edge[DIR_AHEAD].dest;
								 									 dist  = trkSvrData.trackA[expectedSensor].reverse->edge[DIR_AHEAD].dist;
													
								nextSensorNode = getNextSensorNode(aheadOfReverseOfExpectedSensor, &dist, trkSvrData.switchesStatus);
								
								nextSensor = nextSensorNode->num;

								if (req.value == nextSensor){
									//trainStatus *trainStat = &(trkSvrData.trainsStatus[trainIdx]);
									//int   trainActualSpeed = getTrainActualData(TRAIN_ACTUALDATA_SPEED, 
									//					&trkSvrData, trainStat->trainNum, trainStat->currentTrainSpeed);	

									//trainStat->expectedSensorTime += (dist * 10000 / trainActualSpeed);
									break;
								}	
							}						
						}	
					}
					
					if (trainIdx == MAX_TRAINS){
						//sprintf(COM2, "%s\033[45;0H%s Can't distribute sensor %c%d, drop this sensor! %s", 
						//					save, clearLine, (char)sensorGroup, sensorId, restore);
					}
					else{										
						trainStatus *trainStat = &(trkSvrData.trainsStatus[trainIdx]);
							
						/*
						 * Expected time(17;4HExpected time:)
						 */
						int expt = trainStat->expectedSensorTime + trainStat->lastTimeStemp;
						ds = expt / 10 % 10;
						s = expt / 100 % 60;
						m = expt / 6000 % 60;
						h = expt / 360000;
						sprintf(COM2, "%s\033[17;22H%s%s%d:%d:%d:%d%s%s", 
							save, yellow, clearLine, h, m, s, ds, resetColor, restore);
						/*
						 * Time difference(18;4HTime difference:)
						 */				 
						int diff = expt - req.ts;

						if (diff > -30){
							trainStat->delayOffset = trainStat->delayOffset - diff;
						}

						sprintf(COM2, "%s\033[18;22H%s%s%d ticks%s%s", 
							save, yellow, clearLine, diff, resetColor, restore);
						
						
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

						ds = t / 10 % 10;
						s = t / 100 % 60;
						m = t / 6000 % 60;
						h = t / 360000;
						sprintf(COM2, "%s%s\033[13;16H%s%s\033[13;21Hat: %d:%d:%d:%d%s", 
							save, yellow, clearLine, nextSensorNode->name, h, m, s, ds, restore);			
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
				int curVelocity;
				int displacement;
				
				trainStatus *trainStat = &(trkSvrData.trainsStatus[req.target-45]);
				int 		 trainNum = req.target;
				int 		 trainSpeed = trainStat->currentTrainSpeed;
				int 		 prevTrainSpeed = trainStat->previousTrainSpeed;
				int 		 speedChangeTime = trainStat->lastSpeedChange;
				//int i = trkSvrData.currentTrain; 	//for demo 1
				int sensorTimeStemp = trainStat->lastTimeStemp; //trkSvrData.trainsStatus[i].lastTimeStemp;
				locationInfo locInfo;

				if (trainSpeed == 0 && prevTrainSpeed == 0){
					locInfo.displacement = trainStat->currentDisplacement;
					locInfo.sensor = trainStat->lastTriggeredSensor;
					
					Reply(requester, &locInfo, sizeof(locationInfo));
					break;
				}

				if (trainSpeed > 0){
					curVelocity = getTrainActualData(TRAIN_ACTUALDATA_SPEED, &trkSvrData, trainNum, trainSpeed);
					if (prevTrainSpeed == 0 && speedChangeTime > sensorTimeStemp){
						displacement = (Time() - speedChangeTime) * curVelocity / 10000 + trainStat->currentDisplacement;
					}
					else{
						displacement = (Time() - sensorTimeStemp) * curVelocity / 10000;
					}
				}
				else{
					int prevVelocity = getTrainActualData(TRAIN_ACTUALDATA_SPEED, &trkSvrData, trainNum, prevTrainSpeed);
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
							sprintf(COM2, "%s\033[45;%uH%s%s%s", 
								save, a, clearLine, trkSvrData.trackA[(thisTrainPath->path[i])].name, restore);
							a = a + 6;					
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
								//currentDist += 200;
								if (currentDist > startDist + stopDist){
									totalDist  = currentDist - stopDist - startDist;	
									totalDelay = (totalDist * 10000 / actualSpeed) + startTime;// + curvesNum * 10;
								}
								else{
									totalDist  = currentDist;
									totalDelay = getTrainShortMoveTicks(trainNo, trainSpeed, totalDist);
								}

								sprintf(COM2, "%s\033[47;0H totalDist:%d totalDelay:%dcurvesNum:%d %s", 
									save, totalDist, totalDelay, curvesNum, restore);
									
								req.commands[req.numberOfCommands] = (trainWorkerCommand){DELAY_STOP, totalDelay};
								req.numberOfCommands++;	


								int reverseDelay;
								if (nextNode == destNode){
									//reverseDelay = 0;
									
									req.commands[req.numberOfCommands] = (trainWorkerCommand){REVERSE, -1};
									req.numberOfCommands++;		

									break;
								}
								else{
									//reverseDelay = getTrainShortMoveTicks(trainNo, trainSpeed, 200);

									req.commands[req.numberOfCommands] = (trainWorkerCommand){REVERSE, -1};
									req.numberOfCommands++;	

									currentDist = 400 ;//200;
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

								//sprintf(COM2, "%s\033[47;0H totalDist:%d totalDelay:%dcurvesNum:%d %s", 
								//	save, totalDist, totalDelay, curvesNum, restore);
									
								req.commands[req.numberOfCommands] = (trainWorkerCommand){DELAY_STOP, totalDelay};
								req.numberOfCommands++;	
								
								break;						
							}		
					}
					
					/*
					 * 5. trackServer pass this formated path to the trainCommandWorker	
					 */
					Send(thisTrainStatus->trainWorkerTid, &(req), sizeof(trainWorkerRequest), NULL, 0); 
					 					
					
				}
				else{
					//errorMsg;
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

	Delay(300);									//delay 2s

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

