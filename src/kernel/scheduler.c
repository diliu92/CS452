#include <kernel.h>
#include <scheduler.h>

typedef struct kernelGlobal{
	task tasks[MAX_TASK];
	char tasks_stack[MAX_TASK*STACK_SIZE];
	int nextIdleTask;
	int currentActiveTask;
	
	queueItem priorityQueue[MAX_TASK];
	int nextReadyTaskIdx;
	
}kernelGlobal;

static int findEmptyQueueItem(kernelGlobal*);

int popQueue(kernelGlobal* kernelData, int qIdx){
	int tid = -1;
	queueItem* qItem;
	
	if(kernelData->nextReadyTaskIdx != -1){
		qItem = &((kernelData->priorityQueue)[nextReadyTaskIdx])
		tid = qItem->tid;
		
		nextReadyTaskIdx = 	qItem->next == -1 	?	-1 	:	qItem->next;
		
		qItem->tid 	= -1;
		qItem->priority = -1;
		qItem->prev = -1;
		qItem->next	= -1;	
	}
		
	
	return tid;
}

int pushQueue(kernelGlobal* kernelData, int qIdx){
	int qItemIdx = findEmptyQueueItem(kernelData);
	queueItem* pushItem;
	
	int curIdx;
	queueItem* curItem;
	
	
	if (qItemIdx == -1)	//full
		return -1;
	else{
		pushItem = &((kernelData->priorityQueue)[qItemIdx]);
		pushItem->tid = tid;
		pushItem->priority = priority;
		
		if(kernelData->nextReadyTaskIdx == -1){
			pushItem->next = -1;
			kernelData->nextReadyTaskIdx = qItemIdx;
			
			return 1;
		}
		else{
			curIdx = nextReadyTaskIdx;
			do{
				curItem = &((kernelData->priorityQueue)[curIdx]);
				
				if(pushItem->priority >= curItem>priority){
					pushItem->
				}
				
				curIdx = curItem->next;
			}while(curIdx != -1);
			
			
		}
	}
}

static int isQueueEmpty(kernelGlobal* kernelData, int queueIdx){
	priorityQueue* qItem = &((kernelData->priorityQueues)[queueIdx]);
	
	return (qItem->head == NULL && qItem->tail == NULL) ? 1 : 0;
}

