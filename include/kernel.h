#ifndef __KERNEL_H__
#define __KERNEL_H__

#include <task.h>
#include <scheduler.h>

#define MAX_TASK 	64
#define STACK_SIZE	4096	//4KB is the best!

#define MAX_PRIORITY 16

typedef enum taskState_t{
	Idle,
	Ready,
	Active,
	Zombie,
	Send_blocked,
	Receive_blocked,
	Reply_blocked,
	Event_blocked 
}taskState_t;

struct task{
	int tid;
	unsigned int cpsr;
	void* sp;	//a pointer to its private stack
	void* pc;
	
	taskState_t state;	
	int priority;
	int parent_tid;
	
	struct task* nextTask;
};

struct priorityQueue{
	struct task* head;	
	struct task* tail;		
}; 

struct kernelGlobal{
	struct task tasks[MAX_TASK];
	char tasks_stack[MAX_TASK*STACK_SIZE];
	int nextIdleTask;
	int currentActiveTask;
	
	struct priorityQueue priorityQueues[MAX_PRIORITY];	
};

#endif
