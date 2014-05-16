
/*
 * for each field, -1 is the initial value
 * prev: -1 means that this task is the "head"
 * next: -1 means that this task is the "tail"
 */ 
typedef struct queueItem{
	int tid;		//taskID
	int priority;
	int prev;		// 
	int next;		//
}queueItem;


int popQueue(kernelGlobal*,int tid, int priority);

void pushQueue(kernelGlobal*);

