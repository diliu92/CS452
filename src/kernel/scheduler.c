#include <kernel.h>
#include <scheduler.h>


static int isQueueEmpty(kernelGlobal* kernelData, int qIdx);

task* popQueue(kernelGlobal* kernelData, int qIdx){
	priorityQueue* qItem = &((kernelData->priorityQueues)[qIdx]);
	
	task* retval = qItem->head;
	
	qItem->head = (qItem->head)->nextTask;
	
	/*
	 * EQC(Empty Queue Check)
	 */ 
	if(qItem->head == NULL && qItem->tail == retval){	
		qItem->tail == NULL;
	}
	
	((kernelData->whichQueue)[qIdx])--;
			
	return retval;
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
	
	((kernelData->whichQueue)[qIdx])++;
}

int findNextPriorityQueue(kernelGlobal* kernelData){
	int i;
	
	for (i = 0; i < MAX_PRIORITY; i++)
	{
		if ((kernelData->whichQueue)[i] != 0)
			return i;
	}
	return -1;
}

static int isQueueEmpty(kernelGlobal* kernelData, int qIdx){
	priorityQueue* qItem = &((kernelData->priorityQueues)[qIdx]);
	
	return (qItem->head == NULL && qItem->tail == NULL) ? 1 : 0;
}

