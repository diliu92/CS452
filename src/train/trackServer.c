#include <train.h>


typedef struct trainStatus{
	int 	previousTrainSpeed;
	int 	currentTrainSpeed;
	
	int		lastSpeedChange; 		//Time();
	
	int 	trainDirection;
}trainStatus;


typedef struct trackServerData{
	trainStatus trainsStatus[MAX_TRAINS];
	int			switchesStatus[MAX_SWITCHES];
}trackServerData;


static void
initTrackServerData(trackServerData* trkSvrData){
	int i;
	
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
	}
}

static void
changeDirection(int trainNo){
	trkSvrData.trainsStatus[req.target-45].direction = 
		(trkSvrData.trainsStatus[req.target-45].direction == FORWARD) ? BACKWARD : FORWARD;
	return;
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
			case TRACKSERVER_SPEED_GET:
			{			
				Reply(requester, &(trkSvrData.trainsStatus[req.target-45].trainSpeed), sizeof(int));
				break;
			}
			case TRACKSERVER_SPEED_CHANGE:
			{
				trkSvrData.trainsStatus[req.key-45].trainSpeed = req.value % 15;
				
				if (req.value == 15){
					changeDirection(req.target);
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
				changeDirection(req.target);
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
	
	Send(TRACKSERVER_TID, &req, sizeof(trackServerRequest), &(req.retval), sizeof(float));	
	
	return req.retval;
}
void 
changeTrainSpeed(int trainNo, int trainSpeed){
	trackServerRequest req;
	
	putc(COM1, trainSpeed);
	putc(COM1, trainNo);
	
	req.trkSvrReq_uid = TRACKSERVER_SPEED_CHANGE;
		
	req.target 	= trainNo;
	req.value	= trainsSpeed;
	
	Send(TRACKSERVER_TID, &req, sizeof(trackServerRequest), NULL, 0);	
}
void
reverseTrain(int trainNo){
	trackServerRequest req;
	int previousSpeed;

	putc(COM1, 0);								//soft-stop
	putc(COM1, trainNo);
	
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
		
	req.key = switchNo;
	
	Send(TRACKSERVER_TID, &req, sizeof(trackServerRequest), &(req.retval), sizeof(float));	
	
	return (int)req.retval;
}
void
changeSwitchStatus(int switchNo, int switchStatus){
	trackServerRequest req;
	
	putc(COM1,switchStatus);
	putc(COM1,switchNo);
	
	req.trkSvrReq_uid = TRACKSERVER_SWITCH_CHANGE;
		
	req.key 	= switchNo;
	req.value	= (float)switchStatus;
	
	Send(TRACKSERVER_TID, &req, sizeof(trackServerRequest), NULL, 0);	
}
