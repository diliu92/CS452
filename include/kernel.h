#ifndef __KERNEL_H__
#define __KERNEL_H__

#include <task.h>
#include <scheduler.h>

#define MAX_TASK 	64
#define STACK_SIZE	4096	//4KB is the best!

#define MAX_PRIORITY 16


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
