#include <user.h>


static void
idleTask(){
	unsigned int *time4_value = (unsigned int *) TIMER4_LOW;
	
	unsigned int currentTime, lastTime;
	
	unsigned int idleTime	= 0;
	unsigned int totalTime 	= 0;
	
	unsigned int printInterval = 500000;
	
	unsigned int loopTime = 50;
	
	currentTime = *time4_value;
	lastTime 	= *time4_value;

	while (!NeedToShutDown()){
		currentTime = *time4_value;
		
		totalTime += currentTime - lastTime;
		
		if(lastTime + loopTime > currentTime )
			idleTime += currentTime - lastTime;
		
		if( totalTime >= printInterval){
			sprintf( COM2, "%s\033[2;0H%s%sDiff: %d%%s%s", save, clearLine, green, (100*idleTime)/totalTime, restore, resetColor);
			
			totalTime = 0;
			idleTime  = 0;
		}
		
		
		lastTime = currentTime;
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
