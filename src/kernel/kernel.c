#include <kernel.h>
#include <utils.h>



static void tasksInit(struct kernelGlobal* kernelData){
	int i;
	struct task* tsk;
	
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

static void queuesInit(struct kernelGlobal* kernelData){
	int i;
	struct priorityQueue* qItem;
		
	for (i = 0; i < MAX_PRIORITY; i++)
	{
		qItem = &((kernelData->priorityQueues)[i]);
		
		qItem->head = NULL;
		qItem->tail = NULL;
	}	
}

void kernelInit(struct kernelGlobal* kernelData){
	tasksInit(kernelData);
	queuesInit(kernelData);
	/*
	 * More functions to be added.
	 */ 
}


int main( int argc, char* argv[] ) {	
	
	struct kernelGlobal kernelData;
	
	kernelInit(&kernelData);
	
	return 0;
}




