#include <user.h>

static void UART2_SendNotifier(){
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

static void UART2_RecvNotifier(){
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

void UART2_Server(){
	int sendNotifierTid = Create(1, UART2_SendNotifier);
	int recvNotifierTid = Create(1, UART2_RecvNotifier);

	int sendEvtType = UART2_SEND_EVENT;
	int recvEvtType = UART2_RECV_EVENT;

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

	int recvWaitingList[64];
	int recvWaitingListNextReady = 0;
	int recvWaitingListNextFree = 0;
	int recvWaitingListLength = 0;

	int sendReady = 0;
	int curSendTask = -1;

	int sendWaitingListTid[64];
	char sendWaitingListChar[64];
	int sendWaitingListNextReady = 0;
	int sendWaitingListNextFree = 0;
	int sendWaitingListLength = 0;


	int i;
	for (i = 0; i < 64; i++){
		recvWaitingList[i] = 0;
	}

	Send(sendNotifierTid, &sendEvtType, sizeof(int), NULL, 0);

	Send(recvNotifierTid, &recvEvtType, sizeof(int), NULL, 0);

	RegisterAs("UART2 Server");
	for(;;){
		Receive( &requester, &req, sizeof(syscallRequest_UARTServer));
		switch(req.type){
			case TYPE_NOTIFIER_SEND:
				if (sendBufferLength > 0){
					Reply(requester, &(sendBuffer[sendBufferNextReady]), sizeof(char));
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
				while(recvBufferLength > 0 && recvWaitingListLength > 0){

					Reply(recvWaitingList[recvWaitingListNextReady], &recvBuffer[recvBufferNextReady], sizeof(char));
					
					recvBufferNextReady = (recvBufferNextReady + 1) % 4096;
					recvWaitingListNextReady = (recvWaitingListNextReady + 1) % 64;

					recvBufferLength--;
					recvWaitingListLength--;
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
							recvWaitingList[recvWaitingListNextFree] = req.tid;
							recvWaitingListNextFree = (recvWaitingListNextFree + 1) % 64;
							recvWaitingListLength++;
						}
						break;
					case SYSCALL_PUTC:
						if (curSendTask != req.tid && curSendTask > 0){
							sendWaitingListChar[sendWaitingListNextFree] = req.data;
							sendWaitingListTid[sendWaitingListNextFree] = req.tid;
							sendWaitingListNextFree = (sendWaitingListNextFree + 1) % 64;
							sendWaitingListLength++;
						}
						else{
							if (req.data == 19){
								curSendTask = req.tid;
							}
							else if (req.data == 20){
								while (sendWaitingListLength > 0 && sendWaitingListChar[sendWaitingListNextReady] != 19){
									if (sendReady == 1){
										sendReady = 0;
										Reply(sendNotifierTid, &(sendWaitingListChar[sendWaitingListNextReady]), sizeof(char));
									}
									else{
										sendBuffer[sendBufferNextFree] = sendWaitingListChar[sendWaitingListNextReady];
										sendBufferNextFree = (sendBufferNextFree + 1) % 4096;
										sendBufferLength++;
									}
									Reply(sendWaitingListTid[sendWaitingListNextReady], &success, sizeof(int));
									sendWaitingListNextReady = (sendWaitingListNextReady + 1) % 64;
									sendWaitingListLength--;
								}
								if (sendWaitingListLength > 0){
									curSendTask = sendWaitingListTid[sendWaitingListNextReady];
									Reply(sendWaitingListTid[sendWaitingListNextReady], &success, sizeof(int));
									sendWaitingListNextReady = (sendWaitingListNextReady + 1) % 64;
									sendWaitingListLength--;
								}
								else{
									curSendTask = -1;
								}
							}
							else if (sendReady == 1){
								sendReady = 0;
								Reply(sendNotifierTid, &(req.data), sizeof(char));
							}
							else{
								sendBuffer[sendBufferNextFree] = req.data;
								sendBufferNextFree = (sendBufferNextFree + 1) % 4096;
								sendBufferLength++;
							}

							Reply(requester, &success, sizeof(int));
						}						
						break;
				}
				break;
		}
	}

}