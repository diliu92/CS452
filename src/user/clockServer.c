#include <user.h>

static void initTimer(){
	int *timerLoad = (int *) (TIMER3_BASE + LDR_OFFSET);
	int *timerControl = (int *) (TIMER3_BASE + CRTL_OFFSET);
	*timerControl = *timerControl & ~ENABLE_MASK; //disable timer
	*timerLoad = 5080;				 //set timer init value
	//*timerControl = *timerControl & ~MODE_MASK;	 //free runnning
	*timerControl = *timerControl | MODE_MASK;	 //periodic mode
	*timerControl = *timerControl | CLKSEL_MASK; //uncomment to use 508Hz
	*timerControl = *timerControl | ENABLE_MASK; //enable timer
}

static void printTime (int ticks){
	int ds = ticks / 10 % 10;
	int s = ticks / 100 % 60;
	int m = ticks / 6000 % 60;
	int h = ticks / 360000;
	bwprintf( COM2, "%s\033[H%s%sTime: %d:%d:%d:%d%s%s", save, clearLine, green, h, m, s, ds, restore, resetColor);
}

static void clockNotifier(){
	int server;
	int evtType;
	int replyMsg = 0;
	int data;
	syscallRequest_ClockServer req;
	Receive(&server, &evtType, sizeof(int));
	Reply(server, &replyMsg, sizeof(int));
	for(;;){
		data = AwaitEvent(evtType);
		req.type = TYPE_NOTIFIER;
		Send(server, &req, sizeof(syscallRequest_ClockServer), &replyMsg, sizeof(int));
	}
}

void clockServer(){
	RegisterAs("Clock Server");
	int notifierTid = Create(1, clockNotifier);
	bwprintf(COM2, "notifierTid: %d\r\n", notifierTid);
	int tick = 0;
	int evtType = TIMER_EVENT;
	int replyMsg;
	int success = 0;
	int requester;
	syscallRequest_ClockServer req;

	int waitTime[64];

	int numWaitingTask = 0;
	int i;
	for (i = 0; i < 64; i++){
		waitTime[i] = -1;
	}

	Send(notifierTid, &evtType, sizeof(int) ,&replyMsg, sizeof(int));
	initTimer();
	for(;;){
		Receive( &requester, &req, sizeof(syscallRequest_ClockServer));
		switch (req.type){
			case TYPE_NOTIFIER: 
				Reply(requester, &replyMsg, sizeof(int));
				tick++;
				//int c = numWaitingTask;
				if (numWaitingTask > 0){
					for (i = 3; i < 8; i++){
						if (waitTime[i] != -1){
							if (waitTime[i] <= tick){
								Reply(i, &success, sizeof(int));
								waitTime[i] = -1;
								numWaitingTask--;
							}
							//c--;
						}
						/*if (c == 0){
							break;
						}*/
					}
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


/*

// static void printTime (int ticks){
// 	int ds = ticks / 10 % 10;
// 	int s = ticks / 100 % 60;
// 	int m = ticks / 6000 % 60;
// 	int h = ticks / 360000;
// 	bwprintf( COM2, "%s\033[H%s%sTime: %d:%d:%d:%d%s%s", save, clearLine, green, h, m, s, ds, restore, resetColor);
// }

static void clockNotifier(){
	int server;
	int evtType;
	int replyMsg = 0;
	int data;
	syscallRequest_ClockServer req;
	Receive(&server, &evtType, sizeof(int));
	Reply(server, &replyMsg, sizeof(int));
	for(;;){
		data = AwaitEvent(evtType);
		req.type = TYPE_NOTIFIER;
		Send(server, &req, sizeof(syscallRequest_ClockServer), &replyMsg, sizeof(int));
	}
}

void clockServer(){
	RegisterAs("Clock Server");
	int notifierTid = Create(1, clockNotifier);
	bwprintf(COM2, "notifierTid: %d\r\n", notifierTid);
	int tick = 0;
	int evtType = TIMER_EVENT;
	int replyMsg;
	int success = 0;
	int requester;
	syscallRequest_ClockServer req;
	
	// int waitTime[64];
	// int numWaitingTask = 0;
	// int i;
	// for (i = 0; i < 64; i++){
	// 	waitTime[i] = -1;
	// }

	Send(notifierTid, &evtType, sizeof(int) ,&replyMsg, sizeof(int));
	initTimer();
	for(;;){
		Receive( &requester, &req, sizeof(syscallRequest_ClockServer));
		switch (req.type){
			case TYPE_NOTIFIER: 
				Reply(requester, &replyMsg, sizeof(int));
				tick++;
				// if (tick % 10 == 0){
				// 	printTime(tick);
				// }
				//bwprintf(COM2, "Ticks: %d\r\n", tick);
				// int c = numWaitingTask;
				// if (numWaitingTask > 0){
				// 	for (i = 0; i < 64; i++){
				// 		if (waitTime[i] != -1){
				// 			c--;
				// 		}
				// 		else if (waitTime[i] == tick){
				// 			bwprintf(COM2,"tid: %d; tick: %d\r\n", i, tick);
				// 			Reply(i, &success, sizeof(int));
				// 			numWaitingTask--;
				// 		}
				// 		if (c == 0){
				// 			break;
				// 		}
				// 	}
				// }

				break;
			case TYPE_CLIENT:
				switch (req.syscall_uid){
				// 	case SYSCALL_DELAY:
				// 		waitTime[req.tid] = req.ticks + tick;
				// 		numWaitingTask++;
				// 		//bwprintf(COM2,"tid: %d; wait ticks: %d\r\n", req.tid, req.ticks);
				// 		break;
				// 	case SYSCALL_DELAYUNTIL:
				// 		if (req.ticks > tick){
				// 			waitTime[req.tid] = req.ticks;
				// 			numWaitingTask++;							
				// 		}
				// 		else{
				// 			Reply(requester, &success, sizeof(int));
				// 		}
				// 		break;
					case SYSCALL_TIME:
						Reply(requester, &tick, sizeof(int));
						break;
				}
				break;
		}	
	}
}

*/
