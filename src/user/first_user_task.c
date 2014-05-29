#include <user.h>



static void
kernelPartTwo_Sender(){
	char sendMSG[20] = "Hello, Receiver";
	char replyMSG[20];

	bwprintf( COM2, "Sender-%u sent (%s) to Receiver-2\r\n", MyTid(), sendMSG);	
	Send(2, sendMSG, 20, replyMSG, 20);
	bwprintf( COM2, "Sender-%u got a reply(%s) from Receiver-2\r\n", MyTid(), replyMSG);
	
	Exit();
}
static void
kernelPartTwo_Receiver(){
	int tid;
	char recvMSG[20];
	char replyMSG[20] = "Hello, Sender";

	Receieve(&tid,recvMSG,20);
	bwprintf( COM2, "Receiver-%u got a msg(%s) from Sender-%u\r\n", MyTid(), recvMSG, tid);
	bwprintf( COM2, "Receiver-%u sent a reply(%s) from Sender-%u\r\n", MyTid(), replyMSG, tid);
	Reply(tid,replyMSG,20);
	
	Exit();
}

static void
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
}

static void RPS_client1(){
	RPS_clientRequest req;
	req.rpscall_uid = RPSCALL_SIGNUP;
	req.tid = MyTid();
	int target_tid = WhoIs("RPS Server");
	Send(target_tid, &req, sizeof(RPS_clientRequest), &(req.whichTable), sizeof(int));
	bwprintf( COM2, "client1 is sitting on table #%d\r\n", req.whichTable);

	req.rpscall_uid = RPSCALL_PLAY;
	req.data = ROCK;
	Send(target_tid, &req, sizeof(RPS_clientRequest), &(req.retval), sizeof(int));

	req.rpscall_uid = RPSCALL_PLAY;
	req.data = ROCK;
	Send(target_tid, &req, sizeof(RPS_clientRequest), &(req.retval), sizeof(int));

	req.rpscall_uid = RPSCALL_PLAY;
	req.data = ROCK;
	Send(target_tid, &req, sizeof(RPS_clientRequest), &(req.retval), sizeof(int));

	req.rpscall_uid = RPSCALL_PLAY;
	req.data = ROCK;
	Send(target_tid, &req, sizeof(RPS_clientRequest), &(req.retval), sizeof(int));

	Exit();
}

static void RPS_client2(){
	RPS_clientRequest req;
	req.rpscall_uid = RPSCALL_SIGNUP;
	req.tid = MyTid();
	int target_tid = WhoIs("RPS Server");
	Send(target_tid, &req, sizeof(RPS_clientRequest), &(req.whichTable), sizeof(int));
	bwprintf( COM2, "client2 is sitting on table #%d\r\n", req.whichTable);

	req.rpscall_uid = RPSCALL_PLAY;
	req.data = ROCK;
	Send(target_tid, &req, sizeof(RPS_clientRequest), &(req.retval), sizeof(int));

	req.rpscall_uid = RPSCALL_PLAY;
	req.data = PAPER;
	Send(target_tid, &req, sizeof(RPS_clientRequest), &(req.retval), sizeof(int));

	req.rpscall_uid = RPSCALL_PLAY;
	req.data = SCISSORS;
	Send(target_tid, &req, sizeof(RPS_clientRequest), &(req.retval), sizeof(int));

	req.rpscall_uid = RPSCALL_QUIT;
	Send(target_tid, &req, sizeof(RPS_clientRequest), &(req.retval), sizeof(int));

	Exit();
}

static void RPS_client3(){
	RPS_clientRequest req;
	req.rpscall_uid = RPSCALL_SIGNUP;
	req.tid = MyTid();
	int target_tid = WhoIs("RPS Server");
	Send(target_tid, &req, sizeof(RPS_clientRequest), &(req.whichTable), sizeof(int));
	bwprintf( COM2, "client3 is sitting on table #%d\r\n", req.whichTable);

	req.rpscall_uid = RPSCALL_PLAY;
	req.data = SCISSORS;
	Send(target_tid, &req, sizeof(RPS_clientRequest), &(req.retval), sizeof(int));

	req.rpscall_uid = RPSCALL_PLAY;
	req.data = PAPER;
	Send(target_tid, &req, sizeof(RPS_clientRequest), &(req.retval), sizeof(int));

	req.rpscall_uid = RPSCALL_PLAY;
	req.data = ROCK;
	Send(target_tid, &req, sizeof(RPS_clientRequest), &(req.retval), sizeof(int));

	req.rpscall_uid = RPSCALL_QUIT;
	Send(target_tid, &req, sizeof(RPS_clientRequest), &(req.retval), sizeof(int));

	Exit();
}

static void RPS_client4(){
	RPS_clientRequest req;
	req.rpscall_uid = RPSCALL_SIGNUP;
	req.tid = MyTid();
	int target_tid = WhoIs("RPS Server");
	Send(target_tid, &req, sizeof(RPS_clientRequest), &(req.whichTable), sizeof(int));
	bwprintf( COM2, "client4 is sitting on table #%d\r\n", req.whichTable);

	req.rpscall_uid = RPSCALL_PLAY;
	req.data = PAPER;
	Send(target_tid, &req, sizeof(RPS_clientRequest), &(req.retval), sizeof(int));

	req.rpscall_uid = RPSCALL_PLAY;
	req.data = SCISSORS;
	Send(target_tid, &req, sizeof(RPS_clientRequest), &(req.retval), sizeof(int));

	req.rpscall_uid = RPSCALL_PLAY;
	req.data = ROCK;
	Send(target_tid, &req, sizeof(RPS_clientRequest), &(req.retval), sizeof(int));

	req.rpscall_uid = RPSCALL_QUIT;
	Send(target_tid, &req, sizeof(RPS_clientRequest), &(req.retval), sizeof(int));

	Exit();
}


static void
kernelPartTwo(){
	bwprintf( COM2, "\r\n");

	int server = Create(4, RPS_Server);
	int client1 = Create(5, RPS_client1);
	int client2 = Create(5, RPS_client2);
	int client3 = Create(5, RPS_client3);
	int client4 = Create(5, RPS_client4);
	
	Exit();
}

void
firstUserTask()
{
	/* DO NOT CHANGE THE ORDER OF THE FOLLOWING FUNCTION CALLS	*/ 
	
	bwprintf( COM2, "nameServer initialized.\nTID of nameServer: %u(should be 1)\r\n", Create(2,nameServer));		//tid:1
	//clockserver	//tid:2
	//serialserver	//tid:3	
	/* 	                         END							*/
	
	kernelPartTwo();
	
	Exit();
}
