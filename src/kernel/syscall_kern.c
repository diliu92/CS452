
#include <kernel.h>

#include <common/utils.h>


/*
 * Task Related Functions
 * 
 * priority(1 to 16, inclusive)
 */ 

int Task_create(kernGlobal* kernelData, int priority, void (*code)()){
	if(!(priority >=1 && priority <=16))
		return -1;
	if(kernelData->nextTaskUID == MAX_TASK)
		return -2;
	
	task* tsk = &((kernelData->tasks)[kernelData->nextTaskUID]);
	
	tsk->pc = code;
	
	tsk->state = Ready;
	tsk->priority = priority;
	tsk->parent_tid = (kernelData->currentActiveTask == NULL) ?	PARENT_TID_FOR_FIRST_TASK : kernelData->currentActiveTask->tid;
	
	tsk->nextTask = NULL;
	
	Scheduler_pushQueue(kernelData, priority-1, tsk);
	
	return tsk->tid;
}



/*
 * Scheduler Related Functions
 */ 


static int Scheduler_isQueueEmpty(kernGlobal* kernelData, int qIdx){
	priorityQueue* qItem = &((kernelData->priorityQueues)[qIdx]);
	
	return (qItem->head == NULL && qItem->tail == NULL) ? 1 : 0;
}

static int Scheduler_findNextPriorityQueue(kernGlobal* kernelData){
	int i;
	
	for (i = 0; i < MAX_PRIORITY; i++)
	{
		if (Scheduler_isQueueEmpty(kernelData, i))
			continue;
		return i;
	}
	return -1;
}

static task* Scheduler_popQueue(kernGlobal* kernelData, int qIdx){
	priorityQueue* qItem = &((kernelData->priorityQueues)[qIdx]);
	
	task* retval = qItem->head;
	
	qItem->head = (retval)->nextTask;
	
	(retval)->nextTask = NULL;
	/*
	 * EQC(Empty Queue Check)
	 */ 
	if(qItem->head == NULL && qItem->tail == retval){	
		qItem->tail = NULL;
	}
			
	return retval;
}

task* Scheduler_getNextReadyTask(kernGlobal* kernelData){	
	int i = Scheduler_findNextPriorityQueue(kernelData);
	
	if (i >= 0)
		return Scheduler_popQueue(kernelData, i);
			
	return NULL;
}


void Scheduler_pushQueue(kernGlobal* kernelData, int qIdx, task* tsk){
	priorityQueue* qItem = &((kernelData->priorityQueues)[qIdx]);

	if (Scheduler_isQueueEmpty(kernelData, qIdx)){
			qItem->head=tsk;
			qItem->tail=tsk;
	}
	else{
			(qItem->tail)->nextTask = tsk;
			qItem->tail = (qItem->tail)->nextTask;
	}
}

