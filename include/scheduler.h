
#define MAX_PRIORITY 16



/*
 * for each field, -1 is the initial value
 * prev: -1 means that this task is the "head"
 * next: -1 means that this task is the "tail"
 */ 
 
 
typedef struct priorityQueue{
	task* head;		//0-63
	task* tail;		//0-63
}priorityQueue; 
 
 


int popQueue(kernelGlobal*,int tid, int priority);

void pushQueue(kernelGlobal*);

