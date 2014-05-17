#include <task.h>
#include <scheduler.h>


typedef struct kernelGlobal{
	task tasks[MAX_TASK];
	char tasks_stack[MAX_TASK*STACK_SIZE];
	int nextIdleTask;
	int currentActiveTask;
	
	priorityQueue priorityQueues[MAX_PRIORITY];
		
}kernelGlobal;

void kernelInit(kernelGlobal*);



task* getNextReady(kernelGlobal*);

