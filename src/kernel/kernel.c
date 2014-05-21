#include <kernel.h>

int 
main( int argc, char* argv[] ) {	
	int i;
	
	kernGlobal kernelData;
	syscallRequest* req;
	task* active;

	Init(&kernelData);

	bwprintf( COM2, "Hello world.\n\r" );

	for(;;) {
		active = Scheduler_getNextReadyTask(&kernelData);
		if (active != NULL){
			bwprintf( COM2, "Tid=%u\n\r",active->tid);
			//kerxit (active, &req);// req is a pointer to a Request
			bwprintf( COM2, "after kerxit.\n\r" );
			//syscall_kernHandler( &kernelData, req );
		}
	}
	
	return 0;
}




