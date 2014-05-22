#ifndef __SYSCALLKERN_H__
#define __SYSCALLKERN_H__


#include <kernel/kernGlobal.h>




/*
 * Task Related Functions
 * 
 * priority(1 to 16, inclusive)
 */ 

int 
Task_create(kernGlobal* kernelData, int priority, void (*code)());

/*
 * Scheduler Related Functions
 */ 
void 
Scheduler_pushQueue(kernGlobal* kernelData, int qIdx, task* tsk);

task* 
Scheduler_getNextReadyTask(kernGlobal* kernelData);


/*
 * To be added
 */ 




#endif
