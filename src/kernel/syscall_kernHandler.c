
#include <kernel.h>


void 
syscall_kernHandler(kernGlobal* kernelData, syscallRequest* req){
	bwprintf(COM2,"syscall_uid: %u\n", req->syscall_uid);
	switch (req->syscall_uid)
	{
		case SYSCALL_CREATE:
	bwprintf(COM2,"priority: %u\n", ((syscallRequest_Create*)req)->priority);
	bwprintf(COM2,"code: %x\n", ((syscallRequest_Create*)req)->code);		
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
			Scheduler_pushQueue(kernelData, 
									(kernelData->currentActiveTask->priority)-1, 
											kernelData->currentActiveTask);	
			break;
		case SYSCALL_EXIT:
			kernelData->currentActiveTask->state = Zombie;
			break;
	}	
}


