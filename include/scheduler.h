#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <kernel.h>
#include <task.h>

#define MAX_PRIORITY 16
 
struct priorityQueue;
typedef struct priorityQueue priorityQueue; 
  
struct priorityQueue{
	task* head;	
	task* tail;		
}; 
 

task* getNextTask(kernelGlobal* kernelData);

void pushQueue(kernelGlobal* kernelData, int qIdx, task* tsk);


#endif
