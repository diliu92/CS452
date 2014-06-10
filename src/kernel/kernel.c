#include <kernel.h>

int 
main( int argc, char* argv[] ) {	
	
	kernGlobal kernelData;
	syscallRequest* req;
	task* active;

	Init(&kernelData);
	int i;
	for(i = 0; ;i++){
		active = Scheduler_getNextReadyTask(&kernelData);
		//bwprintf(COM2, "active: %d\r\n", active->tid);
		if (active == NULL)
			break;
		//bwprintf(COM2, "before kerxit\r\n");
		req = kerxit (active);
		//bwprintf(COM2, "%d\r\n", i);
		//bwprintf(COM2, "after kerxit\r\n");
		syscall_kernHandler(&kernelData, req);	
	}
	
	int* vic2_clear_addr = (int*)(0x800C0000 + 0x14);
	*vic2_clear_addr = 0xffffffff;

	return 0;
}




