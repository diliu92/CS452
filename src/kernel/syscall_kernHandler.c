

#include <kernel.h>



int syscall_kernHandler(kernGlobal* kernelData, requestMessage* req){
	switch (req->syscall_uid)
	{
		case SYSCALL_CREATE:
			return Task_create(kernelData,
				((requestMessage_Create*)req)->priority, ((requestMessage_Create*)req)->code	);		
		case SYSCALL_MY_TID:
			return kernelData->currentActiveTask->tid;	
		case SYSCALL_MY_PARENT_TID:
			return kernelData->currentActiveTask->parent_tid;	
		case SYSCALL_PASS:
			Scheduler_pushQueue(kernelData, (kernelData->currentActiveTask->priority)-1, kernelData->currentActiveTask);
			return 0;	
		case SYSCALL_EXIT:
			kernelData->currentActiveTask->state = Zombie;
			return 0;
	}
	
	
}
