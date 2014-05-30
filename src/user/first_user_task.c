#include <user.h>

static int start, end;

static void createTimer(){
	int *timerLoad = (int *) (TIMER3_BASE + LDR_OFFSET);
	int *timerControl = (int *) (TIMER3_BASE + CRTL_OFFSET);
	*timerControl = *timerControl & ~ENABLE_MASK; //disable timer
	*timerLoad = 0xFFFFFFFF;				 //set timer init value
	*timerControl = *timerControl & ~MODE_MASK;	 //free runnning
	*timerControl = *timerControl | CLKSEL_MASK; //uncomment to use 508Hz
	*timerControl = *timerControl | ENABLE_MASK; //enable timer
}

static void
speedTest_Sender1(){
	createTimer();
	int *timerValue = (int *) (TIMER3_BASE + VAL_OFFSET);

	start = *timerValue;

	char sendMSG[64] = "123456789.123456789.123456789.123456789.123456789.123456789.123";
	char replyMSG[64];
	Send(3, sendMSG, 64, replyMSG, 64);
	// char sendMSG[4] = "123";
	// char replyMSG[4];
	// Send(3, sendMSG, 4, replyMSG, 4);

	end = *timerValue;
	bwprintf(COM2, "startTime: %u, endTime: %u, timeDifference: %u\r\n", start, end, start - end );
	bwprintf(COM2, "%s\r\n", replyMSG);

	Exit();
}
static void
speedTest_Receiver1(){
	int tid;
	char recvMSG[64];
	char replyMSG[64] = "123456789.123456789.123456789.123456789.123456789.123456789.123";
	Receieve(&tid,recvMSG,64);
	Reply(tid,replyMSG,64);

	// char recvMSG[4];
	// char replyMSG[4] = "123";
	// Receieve(&tid,recvMSG,4);
	// Reply(tid,replyMSG,4);
	
	Exit();
}

static void
speedTest_Sender2(){
	int *timerValue = (int *) (TIMER3_BASE + VAL_OFFSET);

	char sendMSG[64] = "123456789.123456789.123456789.123456789.123456789.123456789.123";
	char replyMSG[64];
	Send(2, sendMSG, 64, replyMSG, 64);

	// char sendMSG[4] = "123";
	// char replyMSG[4];
	// Send(2, sendMSG, 4, replyMSG, 4);
	
	end = *timerValue;
	bwprintf(COM2, "startTime: %u, endTime: %u, timeDifference: %u\r\n", start, end, start - end );
	bwprintf(COM2, "%s\r\n", replyMSG);

	Exit();
}
static void
speedTest_Receiver2(){
	createTimer();
	int *timerValue = (int *) (TIMER3_BASE + VAL_OFFSET);
	bwprintf(COM2, "receive\r\n");
	start = *timerValue;
	int tid;

	char recvMSG[64];
	char replyMSG[64] = "123456789.123456789.123456789.123456789.123456789.123456789.123";
	Receieve(&tid,recvMSG,64);
	Reply(tid,replyMSG,64);

	// char recvMSG[4];
	// char replyMSG[4] = "123";
	// Receieve(&tid,recvMSG,4);
	// Reply(tid,replyMSG,4);
	
	Exit();
}

/*static void
testNameServer(){
	RegisterAs("Yange1");
	RegisterAs("Yang2");
	RegisterAs("Yange3");
	RegisterAs("Yang4");
	RegisterAs("Yange5");
	
	bwprintf( COM2, "%d\r\n", WhoIs("Yange1"));
	bwprintf( COM2, "%d\r\n", WhoIs("Yange5"));
	bwprintf( COM2, "%d\r\n", WhoIs("Yang2"));
	
	Exit();
}*/

static char* replyMap(int choice, char * retval){
	switch(choice)
	{
		case 0: 
			retval = "LOSE";
			break;
		case 1:
			retval = "WIN";
			break;
		case 2: 
			retval = "TIE";
			break;
		case -1:
			retval = "QUITTED";
			break;
	}
	return retval;
}

static void RPS_client1(){
	char* temp;

	RPS_clientRequest req;
	req.rpscall_uid = RPSCALL_SIGNUP;
	req.tid = MyTid();
	int target_tid = WhoIs("RPS Server");
	bwprintf(COM2, "\033[33mPlayer %d:\r\nSending sign up request\r\n", req.tid);
	wait();
	Send(target_tid, &req, sizeof(RPS_clientRequest), &(req.whichTable), sizeof(int));
	bwprintf(COM2, "\033[33mPlayer %d:\r\nRecieved reply: %d (table #)\r\n", req.tid, req.whichTable);
	wait();

	req.rpscall_uid = RPSCALL_PLAY;
	req.data = ROCK;
	bwprintf(COM2, "\033[33mPlayer %d:\r\nSending choice - Round 1: ROCK\r\n", req.tid);
	wait();
	Send(target_tid, &req, sizeof(RPS_clientRequest), &(req.retval), sizeof(int));
	bwprintf(COM2, "\033[33mPlayer %d:\r\nRecieved reply - Round 1: %d (%s)\r\n", req.tid, req.retval, replyMap(req.retval, temp));
	wait();

	req.rpscall_uid = RPSCALL_PLAY;
	req.data = ROCK;
	bwprintf(COM2, "\033[33mPlayer %d:\r\nSending choice - Round 2: ROCK\r\n", req.tid);
	wait();
	Send(target_tid, &req, sizeof(RPS_clientRequest), &(req.retval), sizeof(int));
	bwprintf(COM2, "\033[33mPlayer %d:\r\nRecieved reply - Round 2: %d (%s)\r\n", req.tid, req.retval, replyMap(req.retval, temp));
	wait();

	req.rpscall_uid = RPSCALL_PLAY;
	req.data = ROCK;
	bwprintf(COM2, "\033[33mPlayer %d:\r\nSending choice - Round 3: ROCK\r\n", req.tid);
	wait();
	Send(target_tid, &req, sizeof(RPS_clientRequest), &(req.retval), sizeof(int));
	bwprintf(COM2, "\033[33mPlayer %d:\r\nRecieved reply - Round 3: %d (%s)\r\n", req.tid, req.retval, replyMap(req.retval, temp));
	wait();

	req.rpscall_uid = RPSCALL_QUIT;
	bwprintf(COM2, "\033[33mPlayer %d:\r\nSending quit request\r\n", req.tid);
	wait();
	Send(target_tid, &req, sizeof(RPS_clientRequest), &(req.retval), sizeof(int));
	bwprintf(COM2, "\033[33mPlayer %d:\r\nRecieved reply: %d (%s)\r\n", req.tid, req.retval, replyMap(req.retval, temp));
	wait();

	Exit();
}

static void RPS_client2(){
	char* temp;

	RPS_clientRequest req;
	req.rpscall_uid = RPSCALL_SIGNUP;
	req.tid = MyTid();
	int target_tid = WhoIs("RPS Server");
	bwprintf(COM2, "\033[33mPlayer %d:\r\nSending sign up request\r\n", req.tid);
	wait();
	Send(target_tid, &req, sizeof(RPS_clientRequest), &(req.whichTable), sizeof(int));
	bwprintf(COM2, "\033[33mPlayer %d:\r\nRecieved reply: %d (table #)\r\n", req.tid, req.whichTable);
	wait();

	req.rpscall_uid = RPSCALL_PLAY;
	req.data = ROCK;
	bwprintf(COM2, "\033[33mPlayer %d:\r\nSending choice - Round 1: ROCK\r\n", req.tid);
	wait();

	Send(target_tid, &req, sizeof(RPS_clientRequest), &(req.retval), sizeof(int));
	bwprintf(COM2, "\033[33mPlayer %d:\r\nRecieved reply - Round 1: %d (%s)\r\n", req.tid, req.retval, replyMap(req.retval, temp));
	wait();

	req.rpscall_uid = RPSCALL_PLAY;
	req.data = PAPER;
	bwprintf(COM2, "\033[33mPlayer %d:\r\nSending choice - Round 2: PAPER\r\n", req.tid);
	wait();
	Send(target_tid, &req, sizeof(RPS_clientRequest), &(req.retval), sizeof(int));
	bwprintf(COM2, "\033[33mPlayer %d:\r\nRecieved reply - Round 2: %d (%s)\r\n", req.tid, req.retval, replyMap(req.retval, temp));
	wait();

	req.rpscall_uid = RPSCALL_PLAY;
	req.data = SCISSORS;
	bwprintf(COM2, "\033[33mPlayer %d:\r\nSending choice - Round 3: SCISSORS\r\n", req.tid);
	wait();
	Send(target_tid, &req, sizeof(RPS_clientRequest), &(req.retval), sizeof(int));
	bwprintf(COM2, "\033[33mPlayer %d:\r\nRecieved reply - Round 3: %d (%s)\r\n", req.tid, req.retval, replyMap(req.retval, temp));
	wait();

	req.rpscall_uid = RPSCALL_PLAY;
	req.data = ROCK;
	bwprintf(COM2, "\033[33mPlayer %d:\r\nSending choice - Round 4: ROCK\r\n", req.tid);
	wait();
	Send(target_tid, &req, sizeof(RPS_clientRequest), &(req.retval), sizeof(int));
	bwprintf(COM2, "\033[33mPlayer %d:\r\nRecieved reply: %d (%s)\r\n", req.tid, req.retval, replyMap(req.retval, temp));
	wait();

	Exit();
}

static void RPS_client3(){
	char* temp;

	RPS_clientRequest req;
	req.rpscall_uid = RPSCALL_SIGNUP;
	req.tid = MyTid();
	int target_tid = WhoIs("RPS Server");
	bwprintf(COM2, "\033[33mPlayer %d:\r\nSending sign up request\r\n", req.tid);
	wait();
	Send(target_tid, &req, sizeof(RPS_clientRequest), &(req.whichTable), sizeof(int));
	bwprintf(COM2, "\033[33mPlayer %d:\r\nRecieved reply: %d (table #)\r\n", req.tid, req.whichTable);
	wait();

	req.rpscall_uid = RPSCALL_PLAY;
	req.data = SCISSORS;
	bwprintf(COM2, "\033[33mPlayer %d:\r\nSending choice - Round 1: SCISSORS\r\n", req.tid);
	wait();
	Send(target_tid, &req, sizeof(RPS_clientRequest), &(req.retval), sizeof(int));
	bwprintf(COM2, "\033[33mPlayer %d:\r\nRecieved reply - Round 1: %d (%s)\r\n", req.tid, req.retval, replyMap(req.retval, temp));
	wait();

	req.rpscall_uid = RPSCALL_PLAY;
	req.data = PAPER;
	bwprintf(COM2, "\033[33mPlayer %d:\r\nSending choice - Round 2: PAPER\r\n", req.tid);
	wait();
	Send(target_tid, &req, sizeof(RPS_clientRequest), &(req.retval), sizeof(int));
	bwprintf(COM2, "\033[33mPlayer %d:\r\nRecieved reply - Round 2: %d (%s)\r\n", req.tid, req.retval, replyMap(req.retval, temp));
	wait();

	req.rpscall_uid = RPSCALL_PLAY;
	req.data = PAPER;
	bwprintf(COM2, "\033[33mPlayer %d:\r\nSending choice - Round 3: PAPER\r\n", req.tid);
	wait();
	Send(target_tid, &req, sizeof(RPS_clientRequest), &(req.retval), sizeof(int));
	bwprintf(COM2, "\033[33mPlayer %d:\r\nRecieved reply - Round 3: %d (%s)\r\n", req.tid, req.retval, replyMap(req.retval, temp));
	wait();

	req.rpscall_uid = RPSCALL_QUIT;
	bwprintf(COM2, "\033[33mPlayer %d:\r\nSending quit request\r\n", req.tid);
	wait();
	Send(target_tid, &req, sizeof(RPS_clientRequest), &(req.retval), sizeof(int));
	bwprintf(COM2, "\033[33mPlayer %d:\r\nRecieved reply: %d (%s)\r\n", req.tid, req.retval, replyMap(req.retval, temp));
	wait();

	Exit();
}

static void RPS_client4(){
	char* temp;

	RPS_clientRequest req;
	req.rpscall_uid = RPSCALL_SIGNUP;
	req.tid = MyTid();
	int target_tid = WhoIs("RPS Server");
	bwprintf(COM2, "\033[33mPlayer %d:\r\nSending sign up request\r\n", req.tid);
	wait();
	Send(target_tid, &req, sizeof(RPS_clientRequest), &(req.whichTable), sizeof(int));
	bwprintf(COM2, "\033[33mPlayer %d:\r\nRecieved reply: %d (table #)\r\n", req.tid, req.whichTable);
	wait();

	req.rpscall_uid = RPSCALL_PLAY;
	req.data = PAPER;
	bwprintf(COM2, "\033[33mPlayer %d:\r\nSending choice - Round 1: PAPER\r\n", req.tid);
	wait();
	Send(target_tid, &req, sizeof(RPS_clientRequest), &(req.retval), sizeof(int));
	bwprintf(COM2, "\033[33mPlayer %d:\r\nRecieved reply - Round 1: %d (%s)\r\n", req.tid, req.retval, replyMap(req.retval, temp));
	wait();

	req.rpscall_uid = RPSCALL_PLAY;
	req.data = SCISSORS;
	bwprintf(COM2, "\033[33mPlayer %d:\r\nSending choice - Round 2: SCISSORS\r\n", req.tid);
	wait();
	Send(target_tid, &req, sizeof(RPS_clientRequest), &(req.retval), sizeof(int));
	bwprintf(COM2, "\033[33mPlayer %d:\r\nRecieved reply - Round 2: %d (%s)\r\n", req.tid, req.retval, replyMap(req.retval, temp));
	wait();

	req.rpscall_uid = RPSCALL_PLAY;
	req.data = PAPER;
	bwprintf(COM2, "\033[33mPlayer %d:\r\nSending choice - Round 3: PAPER\r\n", req.tid);
	wait();
	Send(target_tid, &req, sizeof(RPS_clientRequest), &(req.retval), sizeof(int));
	bwprintf(COM2, "\033[33mPlayer %d:\r\nRecieved reply - Round 3: %d (%s)\r\n", req.tid, req.retval, replyMap(req.retval, temp));
	wait();

	req.rpscall_uid = RPSCALL_QUIT;
	bwprintf(COM2, "\033[33mPlayer %d:\r\nSending quit request\r\n", req.tid);
	wait();
	Send(target_tid, &req, sizeof(RPS_clientRequest), &(req.retval), sizeof(int));
	bwprintf(COM2, "\033[33mPlayer %d:\r\nRecieved reply: %d (%s)\r\n", req.tid, req.retval, replyMap(req.retval, temp));
	wait();

	Exit();
}


static void
RPSGame(){
	int server = Create(4, RPS_Server);
	int client1 = Create(5, RPS_client1);
	int client2 = Create(5, RPS_client2);
	int client3 = Create(6, RPS_client3);
	int client4 = Create(6, RPS_client4);
	bwprintf(COM2, "RPS Server tid: %d\r\nCreated player %d, %d, %d, and %d\r\n\r\n", server, client1, client2, client3, client4);
}

static void
speedTest_sendFirst(){
	Create(5, speedTest_Sender1);
	Create(5, speedTest_Receiver1);
}

static void
speedTest_receiveFirst(){
	Create(5, speedTest_Receiver2);
	Create(5, speedTest_Sender2);
}

void
firstUserTask()
{
	/* DO NOT CHANGE THE ORDER OF THE FOLLOWING FUNCTION CALLS	*/ 
	
	bwprintf( COM2, "\r\n\r\nNameServer initialized.\r\nTID of nameServer: %u(should be 1)\r\n", Create(2,nameServer));		//tid:1
	//clockserver	//tid:2
	//serialserver	//tid:3	
	/* 	                         END							*/
	
	RPSGame();
	//speedTest_sendFirst();
	//speedTest_receiveFirst();

	Exit();
}
