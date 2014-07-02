#include <user.h>

//static int start, end;

static unsigned long
readTimer4(){
	unsigned long retval, temp;
	unsigned int *high = (unsigned int *) TIMER4_HIGH;
	unsigned int *low = (unsigned int *) TIMER4_LOW;
	
	retval = *low;
	temp = *high;
	retval = *low + (temp << 32);
	
	return retval;
}


static void
idleTask(){
	unsigned long firstTimer4Read, secondTimer4Read;

	while (!NeedToShutDown()){
		firstTimer4Read  = readTimer4();
		secondTimer4Read = readTimer4();
		
		sprintf( COM2, "%s\033[4;0H%s%sDiff: %d%s%s", save, clearLine, green, secondTimer4Read-firstTimer4Read, restore, resetColor);
	}

	Exit();
}
// void 
// com2_testone(){
// 	while(1){
// 		putc(COM2,'X');
// 	}
// }
// void 
// com2_testtwo(){
// 	while(1){
// 		putc(COM2,getc(COM2));
// 	}
// }

void
firstUserTask()	//priority 3
{
	/* DO NOT CHANGE THE ORDER OF THE FOLLOWING FUNCTION CALLS	*/ 
	int nameServerTID =  Create(2, nameServer);
	//bwprintf( COM2, "NameServer initialized.\r\nTID of NameServer: %u(should be 1)\r\n", nameServerTID);
	int clockServerTID = Create(2, clockServer);
	//bwprintf( COM2, "ClockServer initialized.\r\nTID of ClockServer: %u(should be 2)\r\n", clockServerTID);
	int uart2ServerTID = Create(2, UART2_Server);
	//bwprintf( COM2, "UART2_Server initialized.\r\nTID of UART2_Server: %u(should be 4)\r\n", uart2ServerTID);
	int uart1ServerTID = Create(2, UART1_Server);
	//bwprintf( COM2, "UART1_Server initialized.\r\nTID of UART1_Server: %u(should be 7)\r\n", uart1ServerTID);
	
	sprintf( COM2, "%s\033[H", clearScreen);

	Create(4, sensorFeedProcessor);
	Create(4, cmdProcessor);
	//Create(4, com2_testone);
	//Create(4, com2_testtwo);


	Create(16, idleTask);

	Exit();
}
