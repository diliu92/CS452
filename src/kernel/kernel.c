#include <kernel.h>

#include <common/utils.h>
#include <common/bwio.h>
#include <common/ts7200.h>

static void tasksInit(kernGlobal* kernelData){
	int i;
	task* tsk;
	
	for (i = 0; i < MAX_TASK; i++)
	{
		tsk = &((kernelData->tasks)[i]);
		
		tsk->tid = i;
		tsk->cpsr = 0;
		tsk->sp = (kernelData->tasks_stack)+(STACK_SIZE*(i+1)) - 13; /*r0-r12*/
		tsk->pc = NULL;
		
		tsk->state = Idle;
		tsk->priority = -1;
		tsk->parent_tid = -1;
		
		tsk->nextTask = NULL;
	}
	
	kernelData->nextTaskUID = 0;
	kernelData->currentActiveTask = NULL;	
}

static void queuesInit(kernGlobal* kernelData){
	int i;
	priorityQueue* qItem;
		
	for (i = 0; i < MAX_PRIORITY; i++)
	{
		qItem = &((kernelData->priorityQueues)[i]);
		
		qItem->head = NULL;
		qItem->tail = NULL;
	}	
}

void kerent(){}

void kerxit( task *active, requestMessage **req ) {
	bwprintf( COM2, "kerxit.c: Hello.\n\r" );
	bwprintf( COM2, "kerxit.c: Activating.\n\r" );
	kerent( );
	bwprintf( COM2, "kerxit.c: Good-bye.\n\r" );
}

int main( int argc, char* argv[] ) {	
	
	kernGlobal kernelData;
	requestMessage* req;
	task* active;
	int i;

	bwsetfifo(COM2, OFF);

	tasksInit(&kernelData);
	queuesInit(&kernelData);
	 
	for( i = 0; i < 4; i++ ) {
		active = Scheduler_getNextReadyTask(&kernelData);
		if (active != NULL){
			kerxit (active, &req);// req is a pointer to a Request
			syscall_kernHandler( &kernelData, req );
		}
	}
	
	return 0;
}




