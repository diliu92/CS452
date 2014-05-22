#include <kernel.h>

int 
main( int argc, char* argv[] ) {	
	
	kernGlobal kernelData;
	syscallRequest* req;
	task* active;

	Init(&kernelData);

	bwprintf( COM2, "Hello, world.\n\r" );

	for(;;) {
		active = Scheduler_getNextReadyTask(&kernelData);
		if (active == NULL)
			break;
		req = kerxit (active);
		syscall_kernHandler(&kernelData, req);		
	}
	
	bwprintf( COM2, "Bye, world.\n\r" );
	
	return 0;
}




