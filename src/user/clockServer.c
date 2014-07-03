#include <user.h>

static void printTime (int ticks){
	int ds = ticks / 10 % 10;
	int s = ticks / 100 % 60;
	int m = ticks / 6000 % 60;
	int h = ticks / 360000;

	sprintf( COM2, "%s\033[0;20H%s%sTime: %d:%d:%d:%d%s%s", save, clearLine, green, h, m, s, ds, restore, resetColor);
}

static void clockNotifier(){
	int server;
	int evtType;

	syscallRequest_ClockServer req;
	req.type = TYPE_NOTIFIER;

	Receive(&server, &evtType, sizeof(int));
	Reply(server, NULL, 0);

	for(;;){
		AwaitEvent(evtType, NULL, 0);
		Send(server, &req, sizeof(syscallRequest_ClockServer), NULL, 0);
	}
}

void clockServer(){
	int notifierTid = Create(1, clockNotifier);
	int tick = 0;
	int evtType = TIMER_EVENT;

	int success = 0;
	int requester;
	syscallRequest_ClockServer req;

	int waitTime[64];

	int numWaitingTask = 0;
	int i;
	for (i = 0; i < 64; i++){
		waitTime[i] = -1;
	}
	
	Send(notifierTid, &evtType, sizeof(int) , NULL, 0);

	RegisterAs("Clock Server");
	for(;;){
		Receive( &requester, &req, sizeof(syscallRequest_ClockServer));
		switch (req.type){
			case TYPE_NOTIFIER: 
				Reply(requester, NULL, 0);
				tick++;
				if (numWaitingTask > 0){
					for (i = 4; i < 64; i++){
						if (waitTime[i] != -1){
							if (waitTime[i] <= tick){
								Reply(i, &success, sizeof(int));
								waitTime[i] = -1;
								numWaitingTask--;
							}
						}
					}
				}
				if (tick % 10 == 0){
					printTime(tick);
				}
				break;
			case TYPE_CLIENT:
				switch (req.syscall_uid){
					case SYSCALL_DELAY:
						waitTime[req.tid] = req.ticks + tick;
						numWaitingTask++;	
						break;
					case SYSCALL_DELAYUNTIL:
						if (req.ticks > tick){
							waitTime[req.tid] = req.ticks;
							numWaitingTask++;							
						}
						else{
							Reply(requester, &success, sizeof(int));
						}
						break;
					case SYSCALL_TIME:
						Reply(requester, &tick, sizeof(int));
						break;
				}
				break;
		}	
	}
}
