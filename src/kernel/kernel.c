#include <kernel.h>

int 
main( int argc, char* argv[] ) {	
	int i = 0;
	
	kernGlobal kernelData;
	

	syscallRequest* req;
	task* active;

	Init(&kernelData);

	bwprintf( COM2, "Hello, world.\n\r" );

	for(;;) {
		i++;
		bwprintf(COM2, "loop: %d\n", i);
		active = Scheduler_getNextReadyTask(&kernelData);
		if (active == NULL)
			break;
		bwprintf(COM2, "active is not null\n");
		req = kerxit (active, req);// req is a pointer to a Request
		bwprintf(COM2, "before handler\n");
		syscall_kernHandler( &kernelData, req );
		bwprintf(COM2, "after handler\n");
		
	}
	
	bwprintf( COM2, "Bye, world.\n\r" );
	
	return 0;
}




