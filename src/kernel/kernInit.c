
#include <kernel.h>


static void
hardwareInit(){
	bwsetfifo(COM2, OFF);
	//add kerent to swi jump table
	int* addr = (int*)0x28;
	*addr = (int)&kerent;
}

static void 
tasksInit(kernGlobal* kernelData){
	int i;
	task* tsk;
	
	for (i = 0; i < MAX_TASK; i++)
	{
		tsk = &(kernelData->tasks[i]);
		
		tsk->tid = i;
		tsk->cpsr = 0;
		tsk->sp = (kernelData->tasks_stack)+(STACK_SIZE*(i+1)) - 10*4; /*r0,r4-10,fp*/
		tsk->pc = NULL;
		
		tsk->state = Idle;
		tsk->priority = -1;
		tsk->parent_tid = -1;
		
		tsk->nextPriorityQueueTask = NULL;
		
		tsk->sendQueue.head = NULL;
		tsk->sendQueue.tail = NULL;
			
		tsk->nextSendQueueTask = NULL;	
	
		tsk->whyBlocked = NULL;
	}
	
	kernelData->nextTaskUID = 0;
	kernelData->currentActiveTask = NULL;	
}

static void 
queuesInit(kernGlobal* kernelData){
	int i;
	Queue* qItem;
		
	for (i = 0; i < MAX_PRIORITY; i++)
	{
		qItem = &(kernelData->priorityQueues[i]);
		
		qItem->head = NULL;
		qItem->tail = NULL;
	}	
}


void
Init(kernGlobal* kernelData){
	hardwareInit();
	
	tasksInit(kernelData);
	queuesInit(kernelData);
	
	Task_create(kernelData, 1, firstUserTask);	//first_user_task	tid:0
}
