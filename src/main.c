#include <ts7200.h>
#include <io.h>

//#define	TIMER3_LOAD	(unsigned int *)(TIMER3_BASE + LDR_OFFSET)
//#define	TIMER3_VALUE	(unsigned int *)(TIMER3_BASE + VAL_OFFSET)
//#define	TIMER3_CONTROL	(unsigned int *)(TIMER3_BASE + CRTL_OFFSET)

static unsigned int ourDeciseconds;

static int updateTimerOnScreen(){
	//508kHz - > 50800 cycles per 100ms
	unsigned int currDeciseconds = (*TIMER3_LOAD - *TIMER3_VALUE)/50800;	

	if(currDeciseconds > ourDeciseconds){
		ourDeciseconds = currDeciseconds;

		myprintf(COM2,"\033[s");
		myprintf( COM2, "\033[0;60H Timer: %u:%u:%u.%u\033[K"	//hours:mins:secs:decisecs
				,(ourDeciseconds/36000)%24,(ourDeciseconds/600)%60, (ourDeciseconds/10)%60, ourDeciseconds%10);
		myprintf(COM2,"\033[u");
			
		bwputc(COM1, 133);					
	}
  
	return 0;
}
 
static void timer3Init(){
	//	buf = state ? buf | FEN_MASK : buf & ~FEN_MASK;
	 
	*TIMER3_CONTROL &= ~ENABLE_MASK;	//turn off timer3(0)

	*TIMER3_CONTROL = 0;
	
	*TIMER3_CONTROL &= ~MODE_MASK;		//freerun mode(0)	
	*TIMER3_CONTROL |= CLKSEL_MASK;		//508kHz clock(1)
		
	*TIMER3_LOAD = 0xffffffff;
	 
	*TIMER3_CONTROL |= ENABLE_MASK;		//turn on timer3(1)	
}
 
static void mainInit(){
	ourDeciseconds=0;

	bwsetspeed(COM1,2400);
  
	bwsetfifo( COM1, OFF );
	bwsetfifo( COM2, OFF );

	myprintf(COM2,"\033[2J");
	myprintf(COM2,"\033[0;0HCommand:");
	
	myprintf(COM2,"\033[s\033[15;0H\033[KRecently Triggered Sensors:\033[u");
}


int main( int argc, char* argv[] ) {	
	int i;
	char c;

	char buf1[MAX_BUFFER_SIZE];
	char buf2[MAX_BUFFER_SIZE];
	char buf3[COMMAND_BUFFER_SIZE];
	char buf4[5];
	int buf5[80];
	int buf6[156];
	int buf7[6];
  	int diff=0,prev=0,time=0,worst=0;
 
	ioInit(buf1,buf2,buf3,buf4,buf5,buf6,buf7);	
	mainInit();
	
	getc(COM1);
 
	bwputc(COM1, 96);
	trainsSpeedandSwitchInit();

 	timer3Init();
   
	for(;;){
		//time = *TIMER3_VALUE;
		//diff = time - prev;
		//prev = time;
		//if(diff > worst){
		//	worst = diff;
		//	myprintf(COM2,"\033[s\033[20;0H\033[KWorst:%u\033[u",worst);	
		//} 
		if (updateTimerOnScreen())
			;
		if (isChannelRdyToGet(COM1)){
			c=getc(COM1);
			processSensorResult(c);
		}
		if (doesChannelNeedToPut(COM1) && isChannelRdyToPut(COM1)){
			putc(COM1);
			for (i=0;i<500;i++);	//trains_NOP();
		}
		if (doesChannelNeedToPut(COM2) && isChannelRdyToPut(COM2))
			putc(COM2);
		if (isChannelRdyToGet(COM2)){		//rdy to get users' commands
			c=getc(COM2);
			if(processUserCommands(c))
				break;	
		}
	}

	for (i=0;i<500;i++);	//trains_NOP();
	bwputc(COM1, 97);
	putc(COM1);	

	return 0;

}
