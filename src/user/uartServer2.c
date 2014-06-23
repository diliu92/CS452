#include <user.h>

static void UART2_SendNotifier(){
	int server;
	int evtType;
	int replyMsg = 0;
	char c;
	int* data = (int *)(UART2_BASE + UART_DATA_OFFSET);

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

static void UART2_RecvNotifier(){
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

void UART2_Server(){
	int sendNotifierTid = Create(1, UART2_SendNotifier);
	int recvNotifierTid = Create(1, UART2_RecvNotifier);

	int sendEvtType = UART2_SEND_EVENT;
	int recvEvtType = UART2_RECV_EVENT;

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
	//bwprintf( COM2, "UART2_SendNotifier initialized.\r\nTID of UART2_SendNotifier: %u(should be 5)\r\n", sendNotifierTid);

	Send(recvNotifierTid, &recvEvtType, sizeof(int), &replyMsg, sizeof(int));
	//bwprintf( COM2, "UART2_RecvNotifier initialized.\r\nTID of UART2_RecvNotifier: %u(should be 6)\r\n", recvNotifierTid);

	RegisterAs("UART2 Server");
	for(;;){
		Receive( &requester, &req, sizeof(syscallRequest_UARTServer));
		switch(req.type){
			case TYPE_NOTIFIER_SEND:
				if (sendBufferLength > 0){
					Reply(requester, &sendBuffer[sendBufferNextReady], sizeof(char));
					sendBufferNextReady = (sendBufferNextReady + 1) % 4096;
					sendBufferLength--;	
					sendReady = 0;
				}
				else{
					sendReady = 1;
				}

				break;
			case TYPE_NOTIFIER_RECV:
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
							sendReady = 0;
							Reply(sendNotifierTid, &req.data, sizeof(char));
						}
						else{
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