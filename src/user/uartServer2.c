#include <user.h>

static void UART2_SendNotifier(){
	int server;
	int evtType;
	int replyMsg = 0;
	int data;
	syscallRequest_UARTServer req;
	Receive(&server, &evtType, sizeof(int));
	Reply(server, &replyMsg, sizeof(int));
	for(;;){
		data = AwaitEvent(evtType);
		req.type = TYPE_NOTIFIER_SEND;
		req.data = (char)data;
		Send(server, &req, sizeof(syscallRequest_UARTServer), &replyMsg, sizeof(int));
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

	int *data = (int *)(UART2_BASE + UART_DATA_OFFSET);
	int *uart_control_addr = (int *)(UART2_BASE + UART_CTLR_OFFSET);

	Send(sendNotifierTid, &sendEvtType, sizeof(int), &replyMsg, sizeof(int));
	bwprintf( COM2, "UART2_SendNotifier initialized.\r\nTID of UART2_SendNotifier: %u(should be 5)\r\n", sendNotifierTid);

	Send(recvNotifierTid, &sendEvtType, sizeof(int), &replyMsg, sizeof(int));
	bwprintf( COM2, "UART2_RecvNotifier initialized.\r\nTID of UART2_RecvNotifier: %u(should be 6)\r\n", recvNotifierTid);

	RegisterAs("UART2 Server");
	for(;;){
		Receive( &requester, &req, sizeof(syscallRequest_UARTServer));
		switch(req.type){
			case TYPE_NOTIFIER_SEND:
				Reply(requester, &replyMsg, sizeof(int));

				// if (sendBufferLength > 0){
				// 	*data = sendBuffer[sendBufferNextReady];
				// 	sendBufferNextReady = (sendBufferNextReady + 1) % 4096;
				// 	sendBufferLength--;	

				// 	if(sendBufferLength > 0){
				// 		*uart_control_addr = (*uart_control_addr) | TIEN_MASK;
				// 	}			
				// }

				break;
			case TYPE_NOTIFIER_RECV:
				Reply(requester, &replyMsg, sizeof(int));
				recvBuffer[recvBufferNextFree] = req.data;
				recvBufferNextFree = (recvBufferNextFree + 1) % 4096;
				recvBufferLength++;
				while(recvBufferLength > 0 && waitingListLength > 0){
					Reply(waitingList[waitingListNextReady], &recvBuffer[recvBufferNextReady], sizeof(char));
					
					recvBufferNextReady++;
					waitingListNextReady++;

					recvBufferLength--;
					waitingListLength--;
				}
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
							waitingListNextFree++;
							waitingListLength++;
						}
						break;
					case SYSCALL_PUTC:
						sendBuffer[sendBufferNextFree] = req.data;
						sendBufferNextFree = (sendBufferNextFree + 1) % 4096;
						sendBufferLength++;

						Reply(requester, &success, sizeof(int));

						//turn on transmit intr
						*uart_control_addr = (*uart_control_addr) | TIEN_MASK;
						break;
				}
				break;
		}
	}

}