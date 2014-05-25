
#include <user.h>


/*
static void
kernelPartOne_code(){
	bwprintf( COM2, "My TID: %u, My Parent TID:%u\r\n", MyTid(), MyParentTid());
	Pass();
	bwprintf( COM2, "My TID: %u, My Parent TID:%u\r\n", MyTid(), MyParentTid());
	Exit();	
}



static void
kernelPartOne(){	//priority of FUT is 3
	bwprintf( COM2, "\r\n");
	int Tid_low1 = Create(4,kernelPartOne_code);
	bwprintf( COM2, "Created: %u\r\n", Tid_low1);
		
	int Tid_low2 = Create(4,kernelPartOne_code);
	bwprintf( COM2, "Created: %u\r\n", Tid_low2);
	
	int Tid_high1 = Create(2,kernelPartOne_code);
	bwprintf( COM2, "Created: %u\r\n", Tid_high1);
		
	int Tid_high2 = Create(2,kernelPartOne_code);
	bwprintf( COM2, "Created: %u\r\n", Tid_high2);	
	
	bwprintf( COM2, "First:exiting\r\n" );
	
	Exit();	
}
*/

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
kernelPartTwo(){
	bwprintf( COM2, "\r\n");
	
	int Tid_low1 = Create(4,kernelPartTwo_Sender);		//1
		
	int Tid_low2 = Create(4,kernelPartTwo_Receiver); 	//2
	
	Exit();
}

static void
test(){
	bwprintf( COM2, "\r\n");
	//char testing[20] = "";
	
	bwprintf( COM2, "size of empty string:%u\r\n", strLen(""));
	
	Exit();
}


void
firstUserTask()
{
	/* DO NOT CHANGE THE ORDER OF THE FOLLOWING FUNCTION CALLS	*/ 
	
	//bwprintf( COM2, "nameServer initialized.\nTID of nameServer: %u(should be 1)\r\n", Create(2,nameServer));		//tid:1
	//clockserver	//tid:2
	//serialserver	//tid:3
	
	/* 	                         END							*/
	int Tid_low1 = Create(4,test);		//1
	//kernelPartTwo();
	
}
