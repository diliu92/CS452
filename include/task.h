#ifndef __TASK_H__
#define __TASK_H__


#define MAX_TASK 	64
#define STACK_SIZE	4096	//4KB is the best!

enum taskState_t;
typedef enum taskState_t taskState_t;

enum taskState_t{
	Idle,
	Ready,
	Active,
	Zombie,
	Send_blocked,
	Receive_blocked,
	Reply_blocked,
	Event_blocked 
};


struct task;
typedef struct task task;

struct task{
	int tid;
	unsigned int cpsr;
	void* sp;	//a pointer to its private stack
	void* pc;
	
	taskState_t state;	
	int priority;
	int parent_tid;
	
	task* nextTask;
};

int CreateTask(int, void (*)());



#endif
