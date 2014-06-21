
#include <kernel.h>

static void 
timerInterruptHandler(kernGlobal* kernelData){
	int *timerClear = (int *) (TIMER3_BASE + CLR_OFFSET);
	*timerClear = *timerClear | 1;
	task *clockNotifierTask = &(kernelData->tasks[CLOCK_NOTIFIER_TID]);
	Scheduler_pushQueue(kernelData, clockNotifierTask->priority-1, clockNotifierTask);
}

static void 
uartInterruptHandler(kernGlobal* kernelData, int base){
	int *data = (int *)(base + UART_DATA_OFFSET);
	int *flag = (int *)(base + UART_FLAG_OFFSET);
	int *intr = (int *)(base + UART_INTR_OFFSET);
	int *rsr = (int *)(base + UART_RSR_OFFSET);
	int notifierTid;
	unsigned char c;

	if((base == UART1_BASE) && ((*intr) & MIS_MASK)){	//modem interrupt

	}
	else if((*intr) & RIS_MASK){	//receive buffer full
		switch(base){
			case UART1_BASE:
				notifierTid = 9;
			case UART2_BASE:
				notifierTid = 6;
		} 
		c = *data;
		task *notifierTask = &(kernelData->tasks[notifierTid]);
		notifierTask->whyBlocked->retval = c;
		Scheduler_pushQueue(kernelData, notifierTask->priority-1, notifierTask);
	}
	else if((*intr) & TIS_MASK){	// transmit interrupt
		switch(base){
			case UART1_BASE:
				notifierTid = 8;
			case UART2_BASE:
				notifierTid = 5;
		} 
		//turn off transmit intr
		int* uart_control_addr = (int *)(base + UART_CTLR_OFFSET);
		*uart_control_addr = (*uart_control_addr) & ~TIEN_MASK;

		task *notifierTask = &(kernelData->tasks[notifierTid]);
		Scheduler_pushQueue(kernelData, notifierTask->priority-1, notifierTask);
	}
	else if((base == UART1_BASE) && kernelData->uart1_send_ready){

	}

}

void 
syscall_kernHandler(kernGlobal* kernelData, syscallRequest* req){

	if (req == 0){
		int* vic2_status_addr = (int*)(0x800C0000 + 0x00);
		if ((*vic2_status_addr & (1 << (TIMER_INTERRUPT % 32))) != 0){
			timerInterruptHandler(kernelData);
		}
		if ((*vic2_status_addr & (1 << (UART1_INTERRUPT % 32))) != 0){
			uartInterruptHandler(kernelData, UART1_BASE);

		}
		if ((*vic2_status_addr & (1 << (UART2_INTERRUPT % 32))) != 0){
			uartInterruptHandler(kernelData, UART2_BASE);
		}		
	}
	else{
		switch (req->syscall_uid)
		{
			case SYSCALL_CREATE:
			{	
				syscallRequest_Create* createReq = (syscallRequest_Create*)req; 
					
				req->retval = Task_create(kernelData, createReq->priority, createReq->code);																					
				break;
			}		
			case SYSCALL_MY_TID:
				req->retval =  kernelData->currentActiveTask->tid;
				break;	
			case SYSCALL_MY_PARENT_TID:
				req->retval =  kernelData->currentActiveTask->parent_tid;
				break;	
			case SYSCALL_PASS:
				break;
			case SYSCALL_EXIT:
				kernelData->currentActiveTask->state = Zombie;
				return;
			case SYSCALL_SEND:
			{
				task* sendTask;
				task* recvTask;			
				
				syscallRequest_Send* sendReq = (syscallRequest_Send*)req;
				syscallRequest_Receive* recvReq; 
				
				if(!(sendReq->Tid >=0 && sendReq->Tid <=63)){
					sendReq->retval = -1;
				}
				else if(kernelData->tasks[sendReq->Tid].state == Idle || kernelData->tasks[sendReq->Tid].state == Zombie ){
					sendReq->retval = -2;		
				}
				else{
					sendTask = kernelData->currentActiveTask;
					recvTask = &(kernelData->tasks[sendReq->Tid]);
					
					sendTask->whyBlocked = sendReq;
					
					if (recvTask->state == Send_blocked){
						recvReq = recvTask->whyBlocked;
										
						memcpy(sendReq->msg, recvReq->msg, sendReq->msglen);
						*(recvReq->Tid) = sendTask->tid;
						recvReq->retval = sendReq->msglen;

						recvTask->whyBlocked = NULL;
						Scheduler_pushQueue(kernelData, recvTask->priority-1, recvTask);	
																	
						sendTask->state = Reply_blocked;					
					}
					else{
						//bwprintf(COM2, "---------------------------------------------------\r\n");			
						Message_pushSendQueue(kernelData, recvTask->tid, sendTask);					
					}
				}
						
				return;	//sender always blocks
			}	
			case SYSCALL_RECEIVE:
			{
				syscallRequest_Send* sendReq;
				syscallRequest_Receive* recvReq = (syscallRequest_Receive*)req;
				
				task* sendTask;
				task* recvTask = kernelData->currentActiveTask;
					
				if (Message_isSendQueueEmpty(kernelData,recvTask->tid)){
					recvTask->state = Send_blocked;
					recvTask->whyBlocked = recvReq;
					
					return;
				}
				else{
					sendTask = Message_popSendQueue(kernelData, recvTask->tid);						
					sendReq = sendTask->whyBlocked;
					
					memcpy(sendReq->msg, recvReq->msg, sendReq->msglen);
					*(recvReq->Tid) = sendTask->tid;
					recvReq->retval = sendReq->msglen;			
				}
				
				break;
			}
			case SYSCALL_REPLY:
			{
				task* sendTask;
				task* replyTask = kernelData->currentActiveTask;
				
				syscallRequest_Send* sendReq;			
				syscallRequest_Reply* replyReq = (syscallRequest_Reply*)req;
					
				if(!(replyReq->Tid >=0 && replyReq->Tid <=63))
					replyReq->retval = -1;
				else if(kernelData->tasks[replyReq->Tid].state == Idle || kernelData->tasks[replyReq->Tid].state == Zombie )
					replyReq->retval = -2;
				else if(kernelData->tasks[replyReq->Tid].state != Reply_blocked)
					replyReq->retval = -3;
				else if( ((syscallRequest_Send*)(kernelData->tasks[replyReq->Tid].whyBlocked))->replylen < replyReq->replylen )
					replyReq->retval = -4;		
				else{
					sendTask = &(kernelData->tasks[replyReq->Tid]);
					sendReq = sendTask->whyBlocked;
					
					memcpy(replyReq->reply, sendReq->reply, replyReq->replylen);
					
					sendReq->retval = replyReq->replylen;
					replyReq->retval = 0; 
					
					sendTask->whyBlocked = NULL; 
					Scheduler_pushQueue(kernelData, (sendTask->priority)-1, sendTask);					
				}					
				
				break;
			}
			case SYSCALL_AWAIT:
			{
				task* eventTask = kernelData->currentActiveTask;

				syscallRequest_Await* awaitReq = (syscallRequest_Await*)req;

				eventTask->whyBlocked = awaitReq;

				switch(awaitReq->eventid){
					case TIMER_EVENT:
						eventTask->state = Event_blocked;
						break;
					case UART1_SEND_EVENT:
						eventTask->state = Event_blocked;
						break;
					case UART1_RECV_EVENT:
						eventTask->state = Event_blocked;
						break;
					case UART2_SEND_EVENT:
						eventTask->state = Event_blocked;
						break;
					case UART2_RECV_EVENT:
						eventTask->state = Event_blocked;
						break;
					default:
						req->retval = -1;
						break;
				}
				return;
			}
			default:
				//invalid syscall_uid
				break;
		}
	}
	
	Scheduler_pushQueue(kernelData, 
							(kernelData->currentActiveTask->priority)-1, 
								kernelData->currentActiveTask);		
}


