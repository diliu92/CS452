#ifndef __SYSCALLKERN_H__
#define __SYSCALLKERN_H__


#include <kernel/kernGlobal.h>







/*
 * Task Related Functions
 * 
 * priority(1 to 16, inclusive)
 */ 

int Task_create(int priority, void (*code)());



/*
 * Scheduler Related Functions
 */ 


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






#endif
