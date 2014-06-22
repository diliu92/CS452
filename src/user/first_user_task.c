#include <user.h>

//static int start, end;

void
idleTask(){
	int x;

	while (1){
		//need to figure out how to exit idleTask
	}
}


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

	Create(16, idleTask);

	Exit();
}
