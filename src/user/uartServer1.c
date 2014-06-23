#include <user.h>

static void UART1_SendNotifier(){
	int server;
	int evtType;
	int replyMsg = 0;

	char c;
	int *data = (int *)(UART1_BASE + UART_DATA_OFFSET);

	int retval;
	syscallRequest_UARTServer req;
	Receive(&server, &evtType, sizeof(int));
	Reply(server, &replyMsg, sizeof(int));

	for(;;){
		retval = AwaitEvent(evtType);

		req.type = TYPE_NOTIFIER_SEND;
		Send(server, &req, sizeof(syscallRequest_UARTServer), &c, sizeof(char));
		
		*data = c;
	}
}

static void UART1_RecvNotifier(){
	int server;
	int evtType;
	int replyMsg = 0;
	int data;
	syscallRequest_UARTServer req;
	Receive(&server, &evtType, sizeof(int));
	Reply(server, &replyMsg, sizeof(int));
	for(;;){
		data = AwaitEvent(evtType);
		req.type = TYPE_NOTIFIER_RECV;
		req.data = (char) data;
		Send(server, &req, sizeof(syscallRequest_UARTServer), &replyMsg, sizeof(int));
	}
}

void UART1_Server(){
	int sendNotifierTid = Create(1, UART1_SendNotifier);
	int recvNotifierTid = Create(1, UART1_RecvNotifier);

	int sendEvtType = UART1_SEND_EVENT;
	int recvEvtType = UART1_RECV_EVENT;

	int replyMsg;
	int success = 0;
	int requester;
	syscallRequest_UARTServer req;

	char sendBuffer[4096];
	char recvBuffer[4096];

	int recvBufferNextFree = 0;
	int recvBufferNextReady = 0;
	int recvBufferLength = 0;
	int sendBufferNextFree = 0;
	int sendBufferNextReady = 0;
	int sendBufferLength = 0;

	int waitingList[64];
	int i;
	for (i = 0; i < 64; i++){
		waitingList[i] = 0;
	}
	int waitingListNextReady = 0;
	int waitingListNextFree = 0;
	int waitingListLength = 0;

	int sendReady = 0;

	Send(sendNotifierTid, &sendEvtType, sizeof(int), &replyMsg, sizeof(int));
	//bwprintf( COM2, "UART1_SendNotifier initialized.\r\nTID of UART1_SendNotifier: %u(should be 8)\r\n", sendNotifierTid);

	Send(recvNotifierTid, &recvEvtType, sizeof(int), &replyMsg, sizeof(int));
	//bwprintf( COM2, "UART1_RecvNotifier initialized.\r\nTID of UART1_RecvNotifier: %u(should be 9)\r\n", recvNotifierTid);

	RegisterAs("UART1 Server");
	for(;;){
		Receive( &requester, &req, sizeof(syscallRequest_UARTServer));
		switch(req.type){
			case TYPE_NOTIFIER_SEND:
				if (sendBufferLength > 0){
					//bwputc(COM2, '3');
					Reply(requester, &sendBuffer[sendBufferNextReady], sizeof(char));
					sendBufferNextReady = (sendBufferNextReady + 1) % 4096;
					sendBufferLength--;	
					sendReady = 0;
				}
				else{
					//bwputc(COM2, '4');
					sendReady = 1;
				}

				break;
			case TYPE_NOTIFIER_RECV:
				//putr(COM2,req.data);
				recvBuffer[recvBufferNextFree] = req.data;
				recvBufferNextFree = (recvBufferNextFree + 1) % 4096;
				recvBufferLength++;
				while(recvBufferLength > 0 && waitingListLength > 0){

					Reply(waitingList[waitingListNextReady], &recvBuffer[recvBufferNextReady], sizeof(char));
					
					recvBufferNextReady = (recvBufferNextReady + 1) % 4096;
					waitingListNextReady = (waitingListNextReady + 1) % 64;

					recvBufferLength--;
					waitingListLength--;
				}

				Reply(requester, &replyMsg, sizeof(int));
				break;
			case TYPE_CLIENT:
				switch(req.syscall_uid){
					case SYSCALL_GETC:
						if (recvBufferLength != 0){
							Reply(requester, &recvBuffer[recvBufferNextReady], sizeof(char));
							recvBufferNextReady = (recvBufferNextReady + 1) % 4096;
							recvBufferLength--;
						}
						else{
							waitingList[waitingListNextFree] = req.tid;
							waitingListNextFree = (waitingListNextFree + 1) % 64;
							waitingListLength++;
						}
						break;
					case SYSCALL_PUTC:
						if (sendReady == 1 ){
							//bwputc(COM2, '1');
							sendReady = 0;
							Reply(sendNotifierTid, &req.data, sizeof(char));
						}
						else{
							//bwputc(COM2, '2');
							sendBuffer[sendBufferNextFree] = req.data;
							sendBufferNextFree = (sendBufferNextFree + 1) % 4096;
							sendBufferLength++;
						}

						Reply(requester, &success, sizeof(int));
						break;
				}
				break;
		}
	}

}