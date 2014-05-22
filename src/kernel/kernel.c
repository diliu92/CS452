#include <kernel.h>

int 
main( int argc, char* argv[] ) {	
	int i;
	
	kernGlobal kernelData;
	syscallRequest* req;
	task* active;

	Init(&kernelData);

	bwprintf( COM2, "Hello, world.\n\r" );

	for(;;) {
		active = Scheduler_getNextReadyTask(&kernelData);
		if (active == NULL)
			break;
			
		bwprintf( COM2, "Tid= %u pc:%x sp:%x \n\r",active->tid,active->pc, active->sp);
		
		kerxit (active, &req);// req is a pointer to a Request
		bwprintf( COM2, "after kerxit.\n\r" );
		syscall_kernHandler( &kernelData, req );
	}
	
	bwprintf( COM2, "Bye, world.\n\r" );
	
	return 0;
}




