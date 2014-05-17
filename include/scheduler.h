#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <kernel.h>
#include <task.h>

#define MAX_PRIORITY 16
  
typedef struct priorityQueue{
	struct task* head;	
	struct task* tail;		
}priorityQueue; 
 
task* getNextTask(struct kernelGlobal* kernelData);

void pushQueue(struct kernelGlobal* kernelData, int qIdx, task* tsk);


#endif
