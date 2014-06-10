
#include <kernel.h>


static void
hardwareInit(){
	asm("mrc p15, 0, r0, c1, c0, 0");
	asm("orr r0, r0, #0x00000002"); //D Cache
	asm("orr r0, r0, #0x00001000"); //I Cache
	asm("mcr p15, 0, r0, c1, c0, 0");

	bwsetfifo(COM2, OFF);
	//add kerent to swi jump table
	int* swi_addr = (int*)0x28;
	*swi_addr = (int)&kerent;

	int* hwi_addr = (int*)0x38;
	*hwi_addr = (int)&hwi_kerent;

	//int* vic1_addr = (int*)0x800B0000;
	int* vic2_enable_addr = (int*)(0x800C0000 + 0x10);

	*vic2_enable_addr = *vic2_enable_addr | 1 << 19; //Timer 3 interrup enable
	
	int *timerLoad = (int *) (TIMER3_BASE + LDR_OFFSET);
	int *timerControl = (int *) (TIMER3_BASE + CRTL_OFFSET);
	*timerControl = *timerControl & ~ENABLE_MASK; //disable timer
	*timerLoad = 5080;				 //set timer init value
	//*timerControl = *timerControl & ~MODE_MASK;	 //free runnning
	*timerControl = *timerControl | MODE_MASK;	 //periodic mode
	*timerControl = *timerControl | CLKSEL_MASK; //uncomment to use 508Hz
	*timerControl = *timerControl | ENABLE_MASK; //enable timer
}

static void 
tasksInit(kernGlobal* kernelData){
	int i;
	task* tsk;
	
	for (i = 0; i < MAX_TASK; i++)
	{
		tsk = &(kernelData->tasks[i]);
		
		tsk->tid = i;
		tsk->cpsr = 0;
		tsk->sp = (kernelData->tasks_stack)+(STACK_SIZE*(i+1)) - 20*4; /*r0,r4-10,fp*/
		tsk->pc = NULL;
		
		tsk->state = Idle;
		tsk->priority = -1;
		tsk->parent_tid = -1;
		
		tsk->nextPriorityQueueTask = NULL;
		
		tsk->sendQueue.head = NULL;
		tsk->sendQueue.tail = NULL;
			
		tsk->nextSendQueueTask = NULL;	
	
		tsk->whyBlocked = NULL;
	}
	
	kernelData->nextTaskUID = 0;
	kernelData->currentActiveTask = NULL;	
}

static void 
queuesInit(kernGlobal* kernelData){
	int i;
	Queue* qItem;
		
	for (i = 0; i < MAX_PRIORITY; i++)
	{
		qItem = &(kernelData->priorityQueues[i]);
		
		qItem->head = NULL;
		qItem->tail = NULL;
	}	
}


void
Init(kernGlobal* kernelData){
	hardwareInit();
	
	kernelData->tasks_stack = 0x400000;
	
	tasksInit(kernelData);
	queuesInit(kernelData);

	Task_create(kernelData, 1, firstUserTask);	//first_user_task	tid:0
}
