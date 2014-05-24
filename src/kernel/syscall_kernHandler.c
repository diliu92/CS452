
#include <kernel.h>


void 
syscall_kernHandler(kernGlobal* kernelData, syscallRequest* req){
	
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
			syscallRequest_Send* sendReq = (syscallRequest_Send*)req;
			
			if(!(sendReq->Tid >=0 && sendReq->Tid <=63))
				sendReq->retval = -1;
			else if(kernelData->tasks[sendReq->Tid].state == Idle || kernelData->tasks[sendReq->Tid].state == Zombie )
				sendReq->retval = -2;		
	/*		if(!( ((syscallRequest_Send*)req)->Tid >=0 && ((syscallRequest_Send*)req)->Tid <=63))
				return -3;
	*/
			else{
				task* sendTask = kernelData->currentActiveTask;
				task* recvTask = &(kernelData->tasks[sendReq->Tid]);
				
				sendTask->whyBlocked = sendReq;
				
				if (recvTask->state == Send_blocked){
					/*
					 * To be added, hard-copy msg
					 */ 
					
					recvTask->state == Ready;
					Scheduler_pushQueue(kernelData, recvTask->priority-1, recvTask);	
																
					sendTask->state = Reply_blocked;					
				}
				else{					
					Message_pushSendQueue(kernelData, recvTask->tid, sendTask);
							
					sendTask->state = Receive_blocked;
				}
				
				return;			
			}
					
			break;
		}	
		case SYSCALL_RECEIVE:
			break;
		case SYSCALL_REPLY:
			
			return;
		default:
			//invalid syscall_uid
			break;
	}
	
	Scheduler_pushQueue(kernelData, 
							(kernelData->currentActiveTask->priority)-1, 
								kernelData->currentActiveTask);		
}


