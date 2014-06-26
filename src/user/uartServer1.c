#include <user.h>

static void UART1_SendNotifier(){
	int server;
	int evtType;

	char c;

	syscallRequest_UARTServer req;
	req.type = TYPE_NOTIFIER_SEND;

	Receive(&server, &evtType, sizeof(int));
	Reply(server, NULL, 0);
	for(;;){
		Send(server, &req, sizeof(syscallRequest_UARTServer), &c, sizeof(char));
		AwaitEvent(evtType, &c, sizeof(char));
	}
}

static void UART1_RecvNotifier(){
	int server;
	int evtType;

	char c;

	syscallRequest_UARTServer req;
	req.type = TYPE_NOTIFIER_RECV;

	Receive(&server, &evtType, sizeof(int));
	Reply(server, NULL, 0);
	for(;;){
		AwaitEvent(evtType, &c, sizeof(char));
		req.data = c;

		Send(server, &req, sizeof(syscallRequest_UARTServer), NULL, 0);
	}
}

void UART1_Server(){
	int sendNotifierTid = Create(1, UART1_SendNotifier);
	int recvNotifierTid = Create(1, UART1_RecvNotifier);

	int sendEvtType = UART1_SEND_EVENT;
	int recvEvtType = UART1_RECV_EVENT;

	int success = 0;
	int requester;

	syscallRequest_UARTServer req;

	char sendBuffer[4096];
	int sendBufferNextFree = 0;
	int sendBufferNextReady = 0;
	int sendBufferLength = 0;

	char recvBuffer[4096];
	int recvBufferNextFree = 0;
	int recvBufferNextReady = 0;
	int recvBufferLength = 0;

	int sendReady = 0;

	int waitingList[64];
	int waitingListNextReady = 0;
	int waitingListNextFree = 0;
	int waitingListLength = 0;

	int i;
	for (i = 0; i < 64; i++){
		waitingList[i] = 0;
	}

	Send(sendNotifierTid, &sendEvtType, sizeof(int), NULL, 0);

	Send(recvNotifierTid, &recvEvtType, sizeof(int), NULL, 0);

	RegisterAs("UART1 Server");
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

				Reply(requester, NULL, 0);
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