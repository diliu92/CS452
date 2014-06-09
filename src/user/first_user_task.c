#include <user.h>

//static int start, end;

void
idleTask(){
	// int x = 0;
	while (1){
		// if (x % 100000 == 0){
		// 	x = 0;
		// 	bwprintf(COM2, "time: %u\r\n", Time());
		// }
		// x++;
		Pass();
	}
}

static void client1(){
	int i;
	//for (i = 0; i < 200; i++){
	for(;;){
		bwprintf(COM2, "Client1 before -- %d: %d\r\n", i, Time());
		//Delay(10);
		bwprintf(COM2, "Client1 after -- %d: %d\r\n", i, Time());
	}
	Exit();
}

static void client2(){
	int i;
	//for (i = 0; i < 87; i++){
	for(;;){
		bwprintf(COM2, "Client2 before -- %d: %d\r\n", i, Time());
		//Delay(23);
		bwprintf(COM2, "Client2 after -- %d: %d\r\n", i, Time());
	}	
	Exit();
}

static void client3(){
	int i;
	//for (i = 0; i < 61; i++){
	for(;;){
		bwprintf(COM2, "Client3 before -- %d: %d\r\n", i, Time());
		//Delay(33);
		bwprintf(COM2, "Client3 after -- %d: %d\r\n", i, Time());
	}	
	Exit();
}

static void client4(){
	int i;
	//for (i = 0; i < 28; i++){
	for(;;){
		bwprintf(COM2, "Client4 before -- %d: %d\r\n", i, Time());
		//Delay(71);
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
	Create(3, client2);
	Create(3, client3);	
	Create(3, client4);

	Create(16, idleTask);

	Exit();
}
