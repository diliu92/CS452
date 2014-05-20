

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
			return Task_create(kernelData,
				((requestMessage_Create*)req)->priority, ((requestMessage_Create*)req)->code	);	
		case SYSCALL_PASS:
			return Task_create(kernelData,
				((requestMessage_Create*)req)->priority, ((requestMessage_Create*)req)->code	);	
		case SYSCALL_EXIT:
			return Task_create(kernelData,
				((requestMessage_Create*)req)->priority, ((requestMessage_Create*)req)->code	);	
	}
	
	
}
