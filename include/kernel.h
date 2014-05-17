#ifndef __KERNEL_H__
#define __KERNEL_H__

#include <task.h>
#include <scheduler.h>

struct kernelGlobal{
	struct task tasks[MAX_TASK];
	char tasks_stack[MAX_TASK*STACK_SIZE];
	int nextIdleTask;
	int currentActiveTask;
	
	struct priorityQueue priorityQueues[MAX_PRIORITY];	
};


void kernelInit(struct kernelGlobal*);

struct task* getNextReady(struct kernelGlobal*);

#endif
