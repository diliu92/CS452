#ifndef __KERNEL_H__
#define __KERNEL_H__

#include <task.h>
#include <scheduler.h>

struct kernelGlobal{
	task tasks[MAX_TASK];
	char tasks_stack[MAX_TASK*STACK_SIZE];
	int nextIdleTask;
	int currentActiveTask;
	
	priorityQueue priorityQueues[MAX_PRIORITY];	
};
typedef struct kernelGlobal kernelGlobal; 

void kernelInit(kernelGlobal*);

task* getNextReady(kernelGlobal*);

#endif
