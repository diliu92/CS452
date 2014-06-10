#include <kernel.h>

int 
main( int argc, char* argv[] ) {	
	
	kernGlobal kernelData;
	syscallRequest* req;
	task* active;

	Init(&kernelData);

	for(;;){
		active = Scheduler_getNextReadyTask(&kernelData);
		//bwprintf(COM2, "active: %d\r\n", active->tid);
		if (active == NULL)
			break;
		//bwprintf(COM2, "before kerxit\r\n");
		req = kerxit (active);
		//bwprintf(COM2, "after kerxit\r\n");
		syscall_kernHandler(&kernelData, req);	
	}
		
	return 0;
}




