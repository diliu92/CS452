#include <user.h>
#include <train.h>


typedef struct trackServerData{
	float 	trainsSpeed[MAX_TRAINS];
	int		switchesStatus[MAX_SWITCHES];
}trackServerData;


static void
initTrackServerData(trackServerData* trkSvrData){
	int i;
	
	for (i = 0; i < MAX_TRAINS; i++)
	{
		(trkSvrData->trainsSpeed)[i] = 0;
		
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
		else if (i == 19 || i == 21)
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
	}
	
}



float
getTrainSpeed(int trainNo){
	return 0;
}
void 
updateTrainSpeed(int trainNo, float trainSpeed){
	
}

int
getSwitchStatus(int switchNo){
	return 0;
}
void
updateSwitchStatus(int switchNo, int switchStatus){
	
}
