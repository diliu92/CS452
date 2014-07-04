#include <train.h>


typedef struct trainStatus{
	float 	trainSpeed;
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
		(trkSvrData->trainsStatus).trainSpeed 		= 0;
		(trkSvrData->trainsStatus).trainDirection	= FORWARD;
		
		putc(COM1, 0); 
		putc(COM1, i + 45);
	}
	for (i = 0; i < MAX_SWITCHES; i++)
	{
		if (i <= 17)
		{
			(trkSvrData->switchesStatus)[i] = CURVED;
			
			putc(COM1, CURVED);
			putc(COM1, i + 1);
		}
		else if (i == 18 || i == 20)
		{
			(trkSvrData->switchesStatus)[i] = CURVED;
			
			putc(COM1, CURVED);
			putc(COM1, i + 135);			
		}
		else if (i == 19 || i == 21)l
		{
			(trkSvrData->switchesStatus)[i] = STRAIGHT;
			
			putc(COM1, STRAIGHT);
			putc(COM1, i + 135);			
		}				
	}
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
				Reply(requester, &(trkSvrData.trainsStatus[req.key-45].trainSpeed), sizeof(float));
				break;
			}
			case TRACKSERVER_SPEED_CHANGE:
			{
				trkSvrData.trainsStatus[req.key-45].trainSpeed = key.val
				
				
				Reply(requester, NULL, 0);									
				break;		
			}	
			case TRACKSERVER_SWITCH_GET:
			{
				if(req.key <= 18)
					Reply(requester, &(trkSvrData.switchesStatus[req.key-1]), sizeof(int));
				else
					Reply(requester, &(trkSvrData.switchesStatus[req.key-135]), sizeof(int));					
				break;	
			}
			case TRACKSERVER_SWITCH_CHANGE:
			{
				if(req.key <= 18)
					trkSvrData.switchesStatus[req.key-1] = req.value;
				else
					trkSvrData.switchesStatus[req.key-135] = req.value;
					
				Reply(requester, NULL, 0);									
				break;	
			}	
		}
	}
	
}



float
getTrainSpeed(int trainNo){
	trackServerRequest req;
		
	req.trkSvrReq_uid = TRACKSERVER_SPEED_GET;
		
	req.key = trainNo;
	
	Send(TRACKSERVER_TID, &req, sizeof(trackServerRequest), &(req.retval), sizeof(float));	
	
	return req.retval;
}
void 
changeTrainSpeed(int trainNo, float trainSpeed){
	trackServerRequest req;
	
	putc(COM1,(int)trainSpeed);
	putc(COM1,trainNo);
	
	req.trkSvrReq_uid = TRACKSERVER_SPEED_CHANGE;
		
	req.key 	= trainNo;
	req.value	= trainsSpeed;
	
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
