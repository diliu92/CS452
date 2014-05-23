#ifndef __KERNELGLOBAL_H__
#define __KERNELGLOBAL_H__

#include <common/syscall_information.h>
#include <common/utils.h>

#define MAX_TASK 	64
#define STACK_SIZE	128*1024	//4KB is the best!

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

typedef struct task{
	int tid;
	unsigned int cpsr;
	void* sp;	
	void* pc;

	taskState_t state;	
	int priority;
	int parent_tid;
	
	struct task* nextPriorityQueueTask;	

	Queue sendQueue;	
	struct task* nextSendQueueTask;	
	
	syscallRequest* whyBlocked;		//Send,Recv,Reply		
}task; 


typedef struct kernGlobal{
	task tasks[MAX_TASK];
	int nextTaskUID;
	
	task* currentActiveTask;
	
	Queue priorityQueues[MAX_PRIORITY];	
	
	char tasks_stack[MAX_TASK*STACK_SIZE];	//should be last member of kernGlobal(memory protection)
}kernGlobal;

#endif
