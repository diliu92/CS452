#include <task.h>


typedef struct kernel_global{
	int tid;
	taskState_t state;
	int priority;
	int parent_tid;
	char* sp;
	
}kernel_global;
