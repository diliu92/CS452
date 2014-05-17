#include <task.h>
#include <kernel.h>



#define MAX_PRIORITY 16
 
 
typedef struct priorityQueue{
	task* head;	
	task* tail;		
}priorityQueue; 
  
void pushQueue(kernelGlobal* kernelData, int qIdx, task* tsk);

task* getNextTask(kernelGlobal* kernel_data);

