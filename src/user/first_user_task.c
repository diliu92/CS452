
#include <user.h>



static void
kernelPartOne_code(){
	bwprintf( COM2, "My TID: %u, My Parent TID:%u\r\n", MyTid(), MyParentTid());
	Pass();
	bwprintf( COM2, "My TID: %u, My Parent TID:%u\r\n", MyTid(), MyParentTid());
	Exit();	
}



void
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


static void
kernelPartTwo_Sender(){
	char sendMSG[2];
	char replyMSG[2];
	Send(2, sendMSG, 2, replyMSG, 2);
	
	bwprintf( COM2, "Send Passed\r\n");
	
	Exit();
}

static void
kernelPartTwo_Receiver(){
	int tid;
	char recvMSG[2];
	char replyMSG[2];
		
	Receieve(&tid,recvMSG,2);
	bwprintf( COM2, "Recv Passed\r\n");
	Reply(1,replyMSG,2);
	bwprintf( COM2, "Reply Passed\r\n");
	
	Exit();
}



void
kernelPartTwo(){
	bwprintf( COM2, "\r\n");
	
	int Tid_low1 = Create(4,kernelPartTwo_Sender);		//1
	//bwprintf( COM2, "Created: %u\r\n", Tid_low1);
		
	int Tid_low2 = Create(5,kernelPartTwo_Receiver); 	//2
	//bwprintf( COM2, "Created: %u\r\n", Tid_low2);
	
	Exit();
}
