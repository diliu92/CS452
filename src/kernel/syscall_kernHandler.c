
#include <kernel.h>



int 
syscall_kernHandler(kernGlobal* kernelData, syscallRequest* req){
	switch (req->syscall_uid)
	{
		case SYSCALL_CREATE:
			req->retval = Task_create(kernelData,
										((syscallRequest_Create*)req)->priority, 
											((syscallRequest_Create*)req)->code);		
		case SYSCALL_MY_TID:
			req->retval =  kernelData->currentActiveTask->tid;	
		case SYSCALL_MY_PARENT_TID:
			req->retval =  kernelData->currentActiveTask->parent_tid;	
		case SYSCALL_PASS:
			Scheduler_pushQueue(kernelData, 
									(kernelData->currentActiveTask->priority)-1, 
											kernelData->currentActiveTask);	
		case SYSCALL_EXIT:
			kernelData->currentActiveTask->state = Zombie;
	}	
}


