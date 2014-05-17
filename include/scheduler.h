#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__


#include <task.h>
#include <kernel.h>

#define MAX_PRIORITY 16
 
struct priorityQueue;
typedef struct priorityQueue priorityQueue; 
  
struct priorityQueue{
	task* head;	
	task* tail;		
}; 
  
void pushQueue(kernelGlobal*,int,task*);

task* getNextTask(kernelGlobal*);



#endif
