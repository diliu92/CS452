
#include <kernel.h>


static void
hardwareInit(){
	bwsetfifo(COM2, OFF);
}

static void 
tasksInit(kernGlobal* kernelData){
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

static void 
queuesInit(kernGlobal* kernelData){
	int i;
	priorityQueue* qItem;
		
	for (i = 0; i < MAX_PRIORITY; i++)
	{
		qItem = &((kernelData->priorityQueues)[i]);
		
		qItem->head = NULL;
		qItem->tail = NULL;
	}	
}


void
Init(kernGlobal* kernelData){
	hardwareInit();
	
	tasksInit(kernelData);
	queuesInit(kernelData);
	
	Task_create(kernelData, 3, kernelPartOne);//first_user_task
}
