

#include <kernel.h>



int syscall_kernHandler(kernGlobal* kernelData, requestMessage* req){
	switch (req->syscall_uid)
	{
		case SYSCALL_CREATE:
			return Task_create(kernelData,
				((requestMessage_Create*)req)->priority, ((requestMessage_Create*)req)->code	);
			
		default:
			
	}
	
	
}
