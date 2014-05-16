#include <task.h>
#include <scheduler.h>


static void tasksInit(kernelGlobal*);
static void queueInit(kernelGlobal*);

void kernelInit(kernelGlobal* kernelData){
	tasksInit(kernelData);
	queueInit(kernelData);
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
	}
	
	
}

static void queueInit(kernelGlobal*){
	int i;
	queueItem* qItem;
	
	for (i = 0; i < MAX_TASK; i++)
	{
		qItem = &((kernelData->priorityQueue)[i]);
		
		qItem->tid = -1;
		qItem->priority = -1;
		qItem->prev = -1;
		qItem->next = -1;
	}	
	nextReadyTaskIdx = -1;
}





task* getNextReady(kernelGlobal* kernel_data){
	task* NextReadyTask = NULL;
	int i;
	
	for (i = 0; i < MAX_TASK; i++)
	{
		if( NextReadyTask == NULL && )
			;
	}
	
}
