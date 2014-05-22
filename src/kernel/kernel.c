#include <kernel.h>

int 
main( int argc, char* argv[] ) {	
	
	kernGlobal kernelData;
	syscallRequest* req;
	task* active;

	Init(&kernelData);

	for(;;) {
		active = Scheduler_getNextReadyTask(&kernelData);
		if (active == NULL)
			break;
		req = kerxit (active);
		syscall_kernHandler(&kernelData, req);		
	}
		
	return 0;
}




