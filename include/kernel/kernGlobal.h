#ifndef __KERNELGLOBAL_H__
#define __KERNELGLOBAL_H__



#define MAX_TASK 	64
#define STACK_SIZE	256*1024	//4KB is the best!

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
	void* sp;	//a pointer to its private stack
	void* pc;
	
	taskState_t state;	
	int priority;
	int parent_tid;
	
	struct task* nextTask;
}task; 

typedef struct priorityQueue{
	task* head;	
	task* tail;		
}priorityQueue; 

typedef struct kernGlobal{
	task tasks[MAX_TASK];
	int nextTaskUID;
	task* currentActiveTask;
	
	priorityQueue priorityQueues[MAX_PRIORITY];	
	
	char tasks_stack[MAX_TASK*STACK_SIZE];	//should be last member of kernGlobal(memory protection)
}kernGlobal;

#endif
