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
 * Message Related Functions
 */ 
 
int 
Message_isSendQueueEmpty(kernGlobal* kernelData, int tid);

task* 
Message_popSendQueue(kernGlobal* kernelData, int tid);

void 
Message_pushSendQueue(kernGlobal* kernelData, int tid, task* tsk);



#endif
