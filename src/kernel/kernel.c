#include <kernel.h>

#include <common/utils.h>
#include <common/bwio.h>
#include <common/ts7200.h>

/*
static void tasksInit(kernGlobal* kernelData){
	int i;
	task* tsk;
	
	for (i = 0; i < MAX_TASK; i++)
	{
		tsk = &((kernelData->tasks)[i]);
		
		tsk->tid = i;
		tsk->cpsr = 0;
		tsk->sp = (kernelData->tasks_stack)+(STACK_SIZE*(i+1));
		tsk->pc = NULL;
		
		tsk->state = Idle;
		tsk->priority = -1;
		tsk->parent_tid = -1;
		
		tsk->nextTask = NULL;
	}
	
	
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
*/

int main( int argc, char* argv[] ) {	
	
	//kernGlobal kernelData;
	
	//tasksInit(&kernelData);
	//queuesInit(&kernelData);
	 
	bwsetfifo( COM2, OFF );
	bwprintf( COM2, "Hello world.\n\r" );
	
	return 0;
}




