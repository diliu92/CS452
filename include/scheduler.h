
#define MAX_PRIORITY 16



/*
 * for each field, -1 is the initial value
 * prev: -1 means that this task is the "head"
 * next: -1 means that this task is the "tail"
 */ 
 
 
typedef struct priorityQueue{
	task* head;	
	task* tail;		
}priorityQueue; 
 
 
 
 
task* popQueue(kernelGlobal* kernelData, int qIdx);

void pushQueue(kernelGlobal* kernelData, int qIdx, task* tsk);

int findNextPriorityQueue(kernelGlobal* kernelData);

