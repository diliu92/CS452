#include <user.h>

//static int start, end;

void
idleTask(){
	int x;

	while (1){
		char c = bwgetc(COM2);
		if (c == 'q'){
			Exit();
		}
	}
}

static void client1(){
	int i;
	int tid = MyTid();
	int receive[2];
	Send(0, NULL, 0, &receive, sizeof(int) * 2);
	bwprintf(COM2, "Client1 (TID: %d) - start tick: %d\r\n", tid, Time());
	for (i = 0; i < receive[1]; i++){
		Delay(receive[0]);
		bwprintf(COM2, "Client1 (TID: %d) - completed delay: %d\t- current tick: %d\r\n", tid, i+1, Time());
	}

	Send(0, NULL, 0, NULL, 0);
	Exit();
}

static void client2(){
	int i;
	int tid = MyTid();
	int receive[2];
	Send(0, NULL, 0, &receive, sizeof(int) * 2);
	bwprintf(COM2, "Client2 (TID: %d) - start tick: %d\r\n", tid, Time());
	for (i = 0; i < receive[1]; i++){
		Delay(receive[0]);
		bwprintf(COM2, "Client2 (TID: %d) - completed delay: %d\t- current tick: %d\r\n", tid, i+1, Time());
	}

	Send(0, NULL, 0, NULL, 0);
	Exit();
}

static void client3(){
	int i;
	int tid = MyTid();
	int receive[2];
	Send(0, NULL, 0, &receive, sizeof(int) * 2);
	bwprintf(COM2, "Client3 (TID: %d) - start tick: %d\r\n", tid, Time());
	for (i = 0; i < receive[1]; i++){
		Delay(receive[0]);
		bwprintf(COM2, "Client3 (TID: %d) - completed delay: %d\t- current tick: %d\r\n", tid, i+1, Time());
	}	

	Send(0, NULL, 0, NULL, 0);
	Exit();
}

static void client4(){
	int i;
	int tid = MyTid();
	int receive[2];
	Send(0, NULL, 0, &receive, sizeof(int) * 2);
	bwprintf(COM2, "Client4 (TID: %d) - start tick: %d\r\n", tid, Time());
	for (i = 0; i < receive[1]; i++){
		Delay(receive[0]);
		bwprintf(COM2, "Client4 (TID: %d) - completed delay: %d\t- current tick: %d\r\n", tid, i+1, Time());
	}

	Send(0, NULL, 0, NULL, 0);
	Exit();
}


void
firstUserTask()
{
	/* DO NOT CHANGE THE ORDER OF THE FOLLOWING FUNCTION CALLS	*/ 
	int nameServerTID =  Create(2, nameServer);
	bwprintf( COM2, "%s\033[HNameServer initialized.\r\nTID of NameServer: %u(should be 1)\r\n", clearScreen, nameServerTID);
	int clockServerTID = Create(2, clockServer);
	bwprintf( COM2, "ClockServer initialized.\r\nTID of ClockServer: %u(should be 2)\r\n", clockServerTID);

	int client1Tid = Create(3, client1);
	int client2Tid = Create(4, client2);
	int client3Tid = Create(5, client3);	
	int client4Tid = Create(6, client4);

	Create(16, idleTask);

	int tid;

	//send parameters to clients
	int i;
	int reply[2];
	for (i = 0; i < 4; i++){
		Receive(&tid, NULL, 0);
		if (tid == client1Tid){
				reply[0] = 10;
				reply[1] = 21;
		}
		else if (tid == client2Tid){
				reply[0] = 23;
				reply[1] = 9;
		}
		else if (tid == client3Tid){
				reply[0] = 33;
				reply[1] = 6;
		}
		else if (tid == client4Tid){
				reply[0] = 71;
				reply[1] = 3;			
		}
		Reply(tid, &reply, sizeof(int) * 2);
	}

	//finishing..
	for (i = 0; i < 4; i++){
		Receive(&tid, NULL, 0);
		Reply(tid, NULL, 0);
		bwprintf(COM2, "Client with TID = %d completed its work\r\n", tid);
	}

	bwprintf(COM2, "FirstUserTask Exits\r\n");
	bwprintf(COM2, "IdleTask still running, please press 'q' to go back to redBoot\r\n");
	Exit();
}
