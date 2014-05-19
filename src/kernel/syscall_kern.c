
#include <kernel.h>


/*
 * Task Related Functions
 * 
 * priority(1 to 16, inclusive)
 */ 

int CreateTask(int priority, void (*code)()){
	return 0;
	
}



/*
 * Scheduler Related Functions
 */ 


static int isQueueEmpty(struct kernelGlobal* kernelData, int qIdx){
	struct priorityQueue* qItem = &((kernelData->priorityQueues)[qIdx]);
	
	return (qItem->head == NULL && qItem->tail == NULL) ? 1 : 0;
}

static int findNextPriorityQueue(struct kernelGlobal* kernelData){
	int i;
	
	for (i = 0; i < MAX_PRIORITY; i++)
	{
		if (isQueueEmpty(kernelData, i))
			continue;
		return i;
	}
	return -1;
}

static struct task* popQueue(struct kernelGlobal* kernelData, int qIdx){
	struct priorityQueue* qItem = &((kernelData->priorityQueues)[qIdx]);
	
	struct task* retval = qItem->head;
	
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

struct task* getNextTask(struct kernelGlobal* kernelData){	
	int i = findNextPriorityQueue(kernelData);
	
	if (i >= 0)
		return popQueue(kernelData, i);
			
	return NULL;
}


void pushQueue(struct kernelGlobal* kernelData, int qIdx, struct task* tsk){
	struct priorityQueue* qItem = &((kernelData->priorityQueues)[qIdx]);

	if (isQueueEmpty(kernelData, qIdx)){
			qItem->head=tsk;
			qItem->tail=tsk;
	}
	else{
			(qItem->tail)->nextTask = tsk;
			qItem->tail = (qItem->tail)->nextTask;
	}
}

