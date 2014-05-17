#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <kernel.h>
#include <task.h>

#define MAX_PRIORITY 16
  
struct priorityQueue{
	struct task* head;	
	struct task* tail;		
}; 
 
struct task* getNextTask(struct kernelGlobal*);

void pushQueue(struct kernelGlobal*, int, struct task*);


#endif
