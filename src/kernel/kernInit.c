
#include <kernel.h>

static void 
TimerInit(){
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
UartInit(int base){
	unsigned int on = 0xffffffff;
	unsigned int off = 0x00000000;
	unsigned int ms_irq;

	//set UART specific values (speed, ms, stp2)
	int *speed_high = (int *)(base + UART_LCRM_OFFSET);
	int *speed_low = (int *)(base + UART_LCRL_OFFSET);
	switch(base){
		case UART1_BASE:
			*speed_high = 0x0;	//uart1 speed: 2400
			*speed_low = 0xbf;
			ms_irq = on;
			break;
		case UART2_BASE:
			*speed_high = 0x0;	//uart2 speed: 115200
			*speed_low = 0x3;
			ms_irq = off;
			break;
	}

	//set line fifo
	int* line = (int*) (base + UART_LCRH_OFFSET);
	int buf = *line;
	buf = buf & ~FEN_MASK;
	*line = buf;

	//set UART control
	int* ctrl = (int *)(base + UART_CTLR_OFFSET);
	int ctrlVal = 	(UARTEN_MASK & on)|		//enable = true
					(MSIEN_MASK & ms_irq)|	//modem status irq: UART1 on, UART2 off
					(RIEN_MASK & on)|		//receive irq on
					(TIEN_MASK & off);		//transmit irq off
	*ctrl = (*ctrl) | ctrlVal;
}

static void
cacheInit(){
	asm("mrc p15, 0, r0, c1, c0, 0");
	asm("orr r0, r0, #0x00000004"); //D Cache
	asm("orr r0, r0, #0x00001000"); //I Cache
	asm("mcr p15, 0, r0, c1, c0, 0");

	asm("MOV ip, #0");
	asm("MCR p15, 0, ip, c7, c7, 0");
}

static void
irqInterruptInit(){
	int* vic1_enable_addr = (int*)(0x800B0000 + 0x10);
	int* vic2_enable_addr = (int*)(0x800C0000 + 0x10);

	*vic2_enable_addr = *vic2_enable_addr | 1 << (TIMER_INTERRUPT % 32); //Timer 3 interrup enable
	*vic2_enable_addr = *vic2_enable_addr | 1 << (UART1_INTERRUPT % 32); //UART1 interrup enable
	*vic2_enable_addr = *vic2_enable_addr | 1 << (UART2_INTERRUPT % 32); //UART2 interrup enable
}

static void
hardwareInit(){
	cacheInit();

	//bwsetfifo(COM2, OFF);

	//add kerent to swi jump table
	int* swi_addr = (int*)0x28;
	*swi_addr = (int)&kerent;

	int* hwi_addr = (int*)0x38;
	*hwi_addr = (int)&hwi_kerent;

	TimerInit();
	UartInit(UART1_BASE);
	UartInit(UART2_BASE);

	irqInterruptInit();
	
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
	kernelData->ctsReady = 1;
	kernelData->txReady = 0;

	tasksInit(kernelData);
	queuesInit(kernelData);

	Task_create(kernelData, 3, firstUserTask);	//first_user_task	tid:0
}
