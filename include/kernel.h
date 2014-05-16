#include <task.h>
#include <scheduler.h>


typedef struct kernelGlobal{
	task tasks[MAX_TASK];
	char tasks_stack[MAX_TASK*STACK_SIZE];
	int nextIdleTask;
	int currentActiveTask;
	
	queueItem priorityQueue[MAX_TASK];
	int nextReadyTaskIdx;
	
}kernelGlobal;

void kernelInit(kernelGlobal*);



task* getNextReady(kernelGlobal*);

