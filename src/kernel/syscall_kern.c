
#include <kernel.h>

/*
 * Task Related Functions
 * 
 * priority(1 to 16, inclusive)
 */ 

int 
Task_create(kernGlobal* kernelData, int priority, void (*code)()){
	if(!(priority >=1 && priority <=16))
		return -1;
	if(kernelData->nextTaskUID == MAX_TASK)
		return -2;
	
	task* tsk = &(kernelData->tasks[kernelData->nextTaskUID]);
	
	tsk->cpsr = 0x50;
	tsk->pc = code;
	
	tsk->state = Ready;
	tsk->priority = priority;
	tsk->parent_tid = (kernelData->currentActiveTask == NULL) ?	0 : kernelData->currentActiveTask->tid;
	
	tsk->nextPriorityQueueTask = NULL;
	
	Scheduler_pushQueue(kernelData, priority-1, tsk);
	
	kernelData->nextTaskUID++;
	
	return tsk->tid;
}



/*
 * Scheduler Related Functions
 */ 

static int 
Scheduler_isQueueEmpty(kernGlobal* kernelData, int qIdx){
	Queue* qItem = &(kernelData->priorityQueues[qIdx]);
	
	return (qItem->head == NULL && qItem->tail == NULL) ? 1 : 0;
}
static int 
Scheduler_findNextPriorityQueue(kernGlobal* kernelData){
	int i;
	
	for (i = 0; i < MAX_PRIORITY; i++)
	{
		if (Scheduler_isQueueEmpty(kernelData, i))
			continue;
		return i;
	}
	return -1;
}

static task* 
Scheduler_popQueue(kernGlobal* kernelData, int qIdx){
	Queue* qItem = &(kernelData->priorityQueues[qIdx]);
	
	task* retval = qItem->head;
	
	qItem->head = retval->nextPriorityQueueTask;
	
	retval->nextPriorityQueueTask = NULL;
	/*
	 * EQC(Empty Queue Check)
	 */ 
	if(qItem->head == NULL && qItem->tail == retval){	
		qItem->tail = NULL;
	}
	
	kernelData->currentActiveTask = retval;

	return retval;
}
void 
Scheduler_pushQueue(kernGlobal* kernelData, int qIdx, task* tsk){
	Queue* qItem = &(kernelData->priorityQueues[qIdx]);

	if (Scheduler_isQueueEmpty(kernelData, qIdx)){
			qItem->head=tsk;
			qItem->tail=tsk;
	}
	else{
			((task*)qItem->tail)->nextPriorityQueueTask = tsk;
			qItem->tail = ((task*)qItem->tail)->nextPriorityQueueTask;
	}
}

task* 
Scheduler_getNextReadyTask(kernGlobal* kernelData){	
	int i = Scheduler_findNextPriorityQueue(kernelData);
	
	if (i >= 0)
		return Scheduler_popQueue(kernelData, i);
			
	return NULL;
}

/*
 * Message Related Functions
 */ 
 
static int 
Message_isSendQueueEmpty(kernGlobal* kernelData, int tid){	//0-63
	task* tsk = &(kernelData->tasks[tid]);
	
	return (tsk->sendQueue.head == NULL && tsk->sendQueue.tail == NULL) ? 1 : 0;
}

static task* 
Message_popSendQueue(kernGlobal* kernelData, int tid){
	void* sendQ = &(kernelData->tasks[tid].sendQueue);
	
	task* retval = sendQ->head;
	
	qItem->head = (retval)->nextPriorityQueueTask;
	
	(retval)->nextPriorityQueueTask = NULL;
	/*
	 * EQC(Empty Queue Check)
	 */ 
	if(qItem->head == NULL && qItem->tail == retval){	
		qItem->tail = NULL;
	}
	
	kernelData->currentActiveTask = retval;

	return retval;
}
void 
Message_pushSendQueue(kernGlobal* kernelData, int qIdx, task* tsk){
	priorityQueue* qItem = &((kernelData->priorityQueues)[qIdx]);

	if (Scheduler_isQueueEmpty(kernelData, qIdx)){
			qItem->head=tsk;
			qItem->tail=tsk;
	}
	else{
			(qItem->tail)->nextPriorityQueueTask = tsk;
			qItem->tail = (qItem->tail)->nextPriorityQueueTask;
	}
}

task* 
Message_getNextReadyTask(kernGlobal* kernelData){	
	int i = Scheduler_findNextPriorityQueue(kernelData);
	
	if (i >= 0)
		return Scheduler_popQueue(kernelData, i);
			
	return NULL;
}

