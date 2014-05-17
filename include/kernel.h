#ifndef __KERNEL_H__
#define __KERNEL_H__

#include <task.h>
#include <scheduler.h>

struct kernelGlobal;
typedef struct kernelGlobal kernelGlobal;

struct kernelGlobal{
	task tasks[MAX_TASK];
	char tasks_stack[MAX_TASK*STACK_SIZE];
	int nextIdleTask;
	int currentActiveTask;
	
	priorityQueue priorityQueues[MAX_PRIORITY];	
};

void kernelInit(kernelGlobal*);

task* getNextReady(kernelGlobal*);

#endif
