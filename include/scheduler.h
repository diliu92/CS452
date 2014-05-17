#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <kernel.h>
#include <task.h>


struct task* getNextTask(struct kernelGlobal*);

void pushQueue(struct kernelGlobal* kernelData, int qIdx, struct task* tsk);


#endif
