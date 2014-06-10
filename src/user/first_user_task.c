#include <user.h>

//static int start, end;

void
idleTask(){
	// int x = 0;
	int *timerValue = (int *) (TIMER3_BASE + VAL_OFFSET);
	while (1){
		// if (x % 100000 == 0){
		// 	x = 0;
		// 	bwprintf(COM2, "time: %u\r\n", Time());
		// }
		// x++;
		// bwprintf(COM2, "tick: %d; timer: %d\r\n", Time(), *timerValue);
		Pass();
	}
}


// static void client1(){
// 	while(1){
// 		Send(6,0,0,0,0);
// 	}
// 	Exit();
// }

// static void client2(){
// 	while(1){
// 		Send(6,0,0,0,0);
// 	}
// 	Exit();
// }

// static void client3(){
// 	while(1){
// 		Send(6,0,0,0,0);
// 	}
// 	Exit();
// }

// static void client4(){
// 	int Tid;
	
// 	while(1){
// 		Receive(&Tid, 0,0 );
// 		Reply(Tid, 0 , 0);
// 		Receive(&Tid, 0,0 );
// 		Reply(Tid, 0 , 0);
// 		Receive(&Tid, 0,0 );
// 		Reply(Tid, 0 , 0);
// 	}
	
// 	Exit();
// }


static void client1(){
	int i;
	for (i = 0; i < 20; i++){
	//for(;;){
		bwprintf(COM2, "Client1 before -- %d: %d\r\n", i, Time());
		Delay(10);
		//Time();
		//Time();
		bwprintf(COM2, "Client1 after -- %d: %d\r\n", i, Time());
	}
	Exit();
}

static void client2(){
	int i;
	for (i = 0; i < 9; i++){
	//for(;;){
		bwprintf(COM2, "Client2 before -- %d: %d\r\n", i, Time());
		//Time();
		//Time();
		Delay(23);
		bwprintf(COM2, "Client2 after -- %d: %d\r\n", i, Time());
	}	
	Exit();
}

static void client3(){
	int i;
	for (i = 0; i < 6; i++){
	//for(;;){
		bwprintf(COM2, "Client3 before -- %d: %d\r\n", i, Time());
		//Time();
		//Time();
		Delay(33);
		bwprintf(COM2, "Client3 after -- %d: %d\r\n", i, Time());
	}	
	Exit();
}

static void client4(){
	int i;
	for (i = 0; i < 3; i++){
	//for(;;){
		bwprintf(COM2, "Client4 before -- %d: %d\r\n", i, Time());
		//Time();
		//Time();
		Delay(71);
		bwprintf(COM2, "Client4 after -- %d: %d\r\n", i, Time());
	}
	Exit();
}


void
firstUserTask()
{
	/* DO NOT CHANGE THE ORDER OF THE FOLLOWING FUNCTION CALLS	*/ 
	int nameServerTID =  Create(2, nameServer);
	bwprintf( COM2, "\r\n\r\nNameServer initialized.\r\nTID of nameServer: %u(should be 1)\r\n", nameServerTID);		//tid:1
	int clockServerTID = Create(2, clockServer);
	
	Create(3, client1);
	Create(4, client2);
	Create(5, client3);	
	Create(6, client4);

	Create(7, idleTask);

	Exit();
}
