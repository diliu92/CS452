#include <kernel.h>



void 
kerent(){}

void 
kerxit( task *active, syscallRequest **req ) {
	kerent( );
}

int 
main( int argc, char* argv[] ) {	
	int i;
	
	kernGlobal kernelData;
	syscallRequest* req;
	task* active;

	Init(&kernelData);
		 
	for( i = 0; i < 4; i++ ) {
		active = Scheduler_getNextReadyTask(&kernelData);
		if (active != NULL){
			kerxit (active, &req);// req is a pointer to a Request
			syscall_kernHandler( &kernelData, req );
		}
	}
	
	return 0;
}




