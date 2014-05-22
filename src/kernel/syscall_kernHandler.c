
#include <kernel.h>


void 
syscall_kernHandler(kernGlobal* kernelData, syscallRequest* req){
	
	switch (req->syscall_uid)
	{
		case SYSCALL_CREATE:		
			req->retval = Task_create(kernelData,
										((syscallRequest_Create*)req)->priority, 
											((syscallRequest_Create*)req)->code);																					
			break;		
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
		default:
			//invalid syscall_uid
			break;
	}
	
	Scheduler_pushQueue(kernelData, 
							(kernelData->currentActiveTask->priority)-1, 
								kernelData->currentActiveTask);		
}


