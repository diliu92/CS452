#include <kernel.h>
#include <scheduler.h>

static int isQueueEmpty(kernelGlobal* kernelData, int qIdx){
	priorityQueue* qItem = &((kernelData->priorityQueues)[qIdx]);
	
	return (qItem->head == NULL && qItem->tail == NULL) ? 1 : 0;
}

static int findNextPriorityQueue(kernelGlobal* kernelData){
	int i;
	
	for (i = 0; i < MAX_PRIORITY; i++)
	{
		if (isQueueEmpty(kernelData, i)
			continue;
		return i;
	}
	return -1;
}

static task* popQueue(kernelGlobal* kernelData, int qIdx){
	priorityQueue* qItem = &((kernelData->priorityQueues)[qIdx]);
	
	task* retval = qItem->head;
	
	qItem->head = (qItem->head)->nextTask;
	
	(qItem->head)->nextTask = NULL;
	/*
	 * EQC(Empty Queue Check)
	 */ 
	if(qItem->head == NULL && qItem->tail == retval){	
		qItem->tail == NULL;
	}
			
	return retval;
}

task* getNextTask(kernelGlobal* kernelData){	
	int i = findNextPriorityQueue(kernelData);
	
	if (i >= 0)
		return popQueue(kernelData, i);
			
	return NULL;
}


void pushQueue(kernelGlobal* kernelData, int qIdx, task* tsk){
	priorityQueue* qItem = &((kernelData->priorityQueues)[qIdx]);

	if (isQueueEmpty(kernelData, qIdx)){
			qItem->head=tsk;
			qItem->tail=tsk;
	}
	else{
			(qItem->tail)->nextTask = tsk;
			qItem->tail = (qItem->tail)->nextTask;
	}
}



