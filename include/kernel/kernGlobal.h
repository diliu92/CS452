#ifndef __KERNELGLOBAL_H__
#define __KERNELGLOBAL_H__

#include <common/syscall_information.h>
#include <common/utils.h>

#define MAX_TASK 		64
#define MAX_PRIORITY 	16

#define STACK_LOCATION 	0x00400000	
#define STACK_SIZE		128*1024	//128KB is the best!

#define MAX_PRIORITY 	16

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
	int 			tid;
	unsigned int 	cpsr;
	void* 			sp;	
	void* 			pc;

	taskState_t 	state;	
	int 			priority;
	int 			parent_tid;
	
	struct task* nextPriorityQueueTask;	

	Queue sendQueue;	
	struct task* nextSendQueueTask;	
	
	syscallRequest* whyBlocked;				
}task; 


typedef struct kernGlobal{
	task 	tasks[MAX_TASK];
	char* 	tasks_stack;

	int 	nextTaskUID;
	task* 	currentActiveTask;
	
	Queue priorityQueues[MAX_PRIORITY];	

	int   uart1_ctsReady;
	int   uart1_txReady;
	char  uart1_txChar;
	char* uart1_recvChar;

	char  uart2_txChar;
	char* uart2_recvChar;

	int isShutDownIssued;
}kernGlobal;

#endif
