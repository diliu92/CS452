#include <task.h>
#include <scheduler.h>


static void tasksInit(kernelGlobal*);
static void queueInit(kernelGlobal*);

void kernelInit(kernelGlobal* kernelData){
	tasksInit(kernelData);
	queuesInit(kernelData);
	/*
	 * More functions to be added.
	 */ 
}

static void tasksInit(kernelGlobal* kernelData){
	int i;
	task* tsk;
	
	for (i = 0; i < MAX_TASK; i++)
	{
		tsk = &((kernelData->tasks)[i]);
		
		tsk->tid = i;
		tsk->cpsr = 0;
		tsk->sp = (kernelData->tasks_stack)+(STACK_SIZE*(i+1));
		tsk->pc = NULL;
		
		tsk->state = Empty;
		tsk->priority = -1;
		tsk->parent_tid = -1;
		
		tsk->nextTask = NULL;
	}
	
	
}

static void queuesInit(kernelGlobal*){
	int i;
	priorityQueue* qItem;
		
	for (i = 0; i < MAX_PRIORITY; i++)
	{
		qItem = &((kernelData->priorityQueues)[i]);
		
		qItem->head = NULL;
		qItem->tail = NULL;
	}	

	for (i = 0; i < MAX_PRIORITY; i++)
	{
		(kernelData->whichQueue)[i] = 0;
	}
}


