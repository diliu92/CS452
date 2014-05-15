/*
 * io.c - non-busy-wait I/O routines for diagnosis
 *
 * Specific to the TS-7200 ARM evaluation board
 *
 */

#include <ts7200.h>
#include <io.h>

/*
 * The UARTs are initialized by RedBoot to the following state
 * 	115,200 bps
 * 	8 bits
 * 	no parity
 * 	fifos enabled
 */

//static char com1_OutputBuffer[MAX_BUFFER_SIZE];	//train commands and sensor requests
static char* com1_OutputBuffer;
static unsigned int com1_OutputNextFreeSpot;
static unsigned int com1_OutputNextRdySpot;

//static char com2_OutputBuffer[MAX_BUFFER_SIZE];	//output to terminal
static char* com2_OutputBuffer;
static unsigned int com2_OutputNextFreeSpot;
static unsigned int com2_OutputNextRdySpot;

//static char com2_CommandBuffer[COMMAND_BUFFER_SIZE];	//user's input
static char* com2_CommandBuffer;	//user's input
static unsigned int com2_CommandBufferLength;

static unsigned int com1_InputCounter;
static unsigned int com1_SensorFlag;

static unsigned int lastSensor;

static char* sensors_Mapping;	//A-B-C-D-E
static unsigned int sensor_pos;

static int* sensors_Buffer;	//6
static unsigned int sensors_Buffer_Index;

static int* trains_SpeedBuffer;	//80

static int* trains_SwitchStates;

static void printSwitchTable();


void ioInit(char* buf1, char* buf2, char* buf3, char* buf4, int* buf5, int* buf6, int* buf7){
	int i;
	com1_OutputBuffer = buf1;
	com2_OutputBuffer = buf2;
	com2_CommandBuffer = buf3;
	sensors_Mapping = buf4;
	trains_SpeedBuffer = buf5;
	trains_SwitchStates = buf6;
 	sensors_Buffer = buf7;

	for(i=0;i<6;i++)
		sensors_Buffer[i] = ' ';


	com1_InputCounter = 0;
	com1_SensorFlag = 1;

	lastSensor = 14;

	sensors_Mapping[0] = 'A';
	sensors_Mapping[1] = 'B';
	sensors_Mapping[2] = 'C';
	sensors_Mapping[3] = 'D';
	sensors_Mapping[4] = 'E';

	sensor_pos=0;

	sensors_Buffer_Index = 0;

	com1_OutputNextFreeSpot = 0;
	com1_OutputNextRdySpot = 0;

	com2_CommandBufferLength = 0;	

	com2_OutputNextFreeSpot = 0;
	com2_OutputNextRdySpot = 0;
}
void trainsSpeedandSwitchInit(){
	int i;

	for(i=45;i<=55;i++){
		bwputc(COM1, 0);
		bwputc(COM1, i);
	}

	for(i=0;i<18;i++){
		trains_SwitchStates[i] = curve;

		bwputc(COM1, curve);
		bwputc(COM1, i+1);
	}
	
	for(i=152;i<156;i++){
		switch(i%2){
			case 0:
				trains_SwitchStates[i] = straight;	

				bwputc(COM1, straight);
				bwputc(COM1, i+1);			
				break;
			case 1:
				trains_SwitchStates[i] = curve;	

				bwputc(COM1, curve);
				bwputc(COM1, i+1);		
				break; 
		}		
	}
	myprintf(COM2,"\033[s");
	printSwitchTable();
	myprintf(COM2,"\033[u");	
}

int doesChannelNeedToPut(int channel){
	switch( channel ) {
	case COM1:
		if(com1_OutputNextFreeSpot != com1_OutputNextRdySpot)
			return 1;	
		break;
	case COM2:
		if(com2_OutputNextFreeSpot != com2_OutputNextRdySpot)
			return 1;
		break;
	}
	
	return 0;
} 

int needToQuerySensors(){
	if  (com1_SensorFlag == 1){
		com1_SensorFlag = 0;
		return 1;
	}
	else
		return 0;
}

int isChannelRdyToPut(int channel){
	int *flags;

	switch( channel ) {
	case COM1:
		flags = (int *)( UART1_BASE + UART_FLAG_OFFSET );		
		if ( !(*flags & TXFF_MASK) && (*flags & CTS_MASK) )	
			return 1;
		break;
	case COM2:
		flags = (int *)( UART2_BASE + UART_FLAG_OFFSET );
		if (!(*flags & TXFF_MASK)) 
			return 1;
		break;
	}
	
	return 0;
} 
int isChannelRdyToGet(int channel){
	int *flags;

	switch( channel ) {
	case COM1:
		flags = (int *)( UART1_BASE + UART_FLAG_OFFSET );
		if ( (*flags & RXFF_MASK) )
			return 1;
		break;
	case COM2:
		flags = (int *)( UART2_BASE + UART_FLAG_OFFSET );
		if ( (*flags & RXFF_MASK) )
			return 1;
		break;
	}

	return 0;	
}  

void putc(int channel){
	unsigned char c;
	int *data;

	switch( channel ) {
	case COM1:
		data = (int *)( UART1_BASE + UART_DATA_OFFSET );
		c = com1_OutputBuffer[com1_OutputNextRdySpot];
		com1_OutputNextRdySpot = (com1_OutputNextRdySpot+1) % MAX_BUFFER_SIZE;

		break;
	case COM2:
		data = (int *)( UART2_BASE + UART_DATA_OFFSET );
		c = com2_OutputBuffer[com2_OutputNextRdySpot];
		com2_OutputNextRdySpot = (com2_OutputNextRdySpot+1) % MAX_BUFFER_SIZE;	

		break;
	}
	*data = c;
}
unsigned char getc(int channel){
	int *data;
	unsigned char c;

	switch( channel ) {
	case COM1:
		data = (int *)( UART1_BASE + UART_DATA_OFFSET );
		break;
	case COM2:;
		data = (int *)( UART2_BASE + UART_DATA_OFFSET );
		break;
	}

	c = *data;
	return c;
}

static void printSwitchTable(){
	int i, uCounter=0;
	
	myprintf(COM2,"\033[3;0H---------------Switches Table--------------\n");

	for(i=0;i<18;i++){
		if(i<9) myprintf(COM2, " ");	 
		switch(trains_SwitchStates[i]){
			case straight:
				myprintf(COM2, "%u: S\t",i+1);			
				break;
			case curve:
				myprintf(COM2, "%u: C\t",i+1);		
				break; 
		}

		uCounter++;
		if(uCounter%6 == 0)
			myprintf(COM2,"\n");			
	}

	for(i=152;i<156;i++){
		switch(trains_SwitchStates[i]){
			case straight:
				myprintf(COM2, "%u: S\t",i+1);			
				break;
			case curve:
				myprintf(COM2, "%u: C\t",i+1);		
				break; 
		}

		uCounter++;
		if(uCounter%6 == 0)
			myprintf(COM2,"\n");			
	}	
}

int processUserCommands(char c){
	int train_num, train_speed, switch_number, swtich_direction;
	int isCorrect = 0;

	if(c != 13){	// \r
		bwputc(COM2, c);

		com2_CommandBuffer[com2_CommandBufferLength] = c;
		com2_CommandBufferLength++;
	}
	else {
		if(com2_CommandBufferLength == 1 && com2_CommandBuffer[0] == 'q')
			return 1;
		else if( (com2_CommandBufferLength>=6 && com2_CommandBufferLength<=8) 
				&& com2_CommandBuffer[0] == 't' && com2_CommandBuffer[1] == 'r'){

			switch(com2_CommandBufferLength){
				case 6:
					train_num = 	com2_CommandBuffer[3] - '0';
					train_speed = 	com2_CommandBuffer[5] - '0';
			
					isCorrect = 1;	
					break;
				case 7:
					if(com2_CommandBuffer[4] == ' '){
						train_num = 	com2_CommandBuffer[3] - '0';
						train_speed = 	(com2_CommandBuffer[5] - '0')*10 + (com2_CommandBuffer[6] - '0');

						isCorrect = 1;
					}
					else{
						train_num = 	(com2_CommandBuffer[3] - '0')*10 + (com2_CommandBuffer[4] - '0');
						train_speed = 	com2_CommandBuffer[6] - '0';	

						isCorrect = 1;				
					}
					break;
				case 8:
					train_num = 	(com2_CommandBuffer[3] - '0')*10 + (com2_CommandBuffer[4] - '0');
					train_speed = 	(com2_CommandBuffer[6] - '0')*10 + (com2_CommandBuffer[7] - '0');

					isCorrect = 1;
					break;
			}

			if(isCorrect){
				bwputc(COM1, train_speed);
				bwputc(COM1, train_num);

				trains_SpeedBuffer[train_num-1] = train_speed;
			}
		}
		else if( (com2_CommandBufferLength>=4 && com2_CommandBufferLength<=5) 
				&& com2_CommandBuffer[0] == 'r' && com2_CommandBuffer[1] == 'v'){
			
			switch(com2_CommandBufferLength){
				case 4:
					train_num = com2_CommandBuffer[3] - '0';
					break;
				case 5:
					train_num = (com2_CommandBuffer[3] - '0')*10 + (com2_CommandBuffer[4] - '0');
					break;
			}

			bwputc(COM1, 15);
			bwputc(COM1, train_num);

			bwputc(COM1, trains_SpeedBuffer[train_num-1]);
			bwputc(COM1, train_num);
		}
		else if( (com2_CommandBufferLength>=6 && com2_CommandBufferLength<=8) 
				&& com2_CommandBuffer[0] == 's' && com2_CommandBuffer[1] == 'w'){

			switch(com2_CommandBufferLength){
				case 6:
					switch_number = com2_CommandBuffer[3] - '0';
					swtich_direction = (com2_CommandBuffer[5] == 'S' || com2_CommandBuffer[5] == 's' )? straight : curve ;

					trains_SwitchStates[switch_number-1] = swtich_direction;
					break;
				case 7:
					switch_number = (com2_CommandBuffer[3] - '0')*10 + (com2_CommandBuffer[4] - '0');
					swtich_direction = (com2_CommandBuffer[6] == 'S' || com2_CommandBuffer[6] == 's' )? straight : curve  ;

					trains_SwitchStates[switch_number-1] = swtich_direction;
					break;
				case 8:
					switch_number = (com2_CommandBuffer[3] - '0')*100 + (com2_CommandBuffer[4] - '0')*10 + (com2_CommandBuffer[5] - '0');
					swtich_direction = (com2_CommandBuffer[7] == 'S' || com2_CommandBuffer[7] == 's' )? straight : curve  ;

					trains_SwitchStates[switch_number-1] = swtich_direction;
					break;

			}

			bwputc(COM1, swtich_direction);
			bwputc(COM1, switch_number);

			myprintf(COM2,"\033[s");
			printSwitchTable();
			myprintf(COM2,"\033[u");
		}
		myprintf(COM2,"\033[0;0HCommand:\033[K");

		com2_CommandBufferLength = 0;
	}
	return 0;	
}
 
static void printSensorsStatus(){
	int i = sensors_Buffer_Index;

	myprintf(COM2,"\033[s");

	myprintf(COM2,"\033[15;30H\033[K ");
	do{
		if (sensors_Buffer[i] != ' ')
			myprintf(COM2,"%c%u  ",sensors_Buffer[i]/100,sensors_Buffer[i]%100);
		else
			myprintf(COM2," ");
		i--;
		if (i<0) i = 5;
	}while(i!=sensors_Buffer_Index);

	myprintf(COM2,"\033[u");
}

int processSensorResult(char c){
	char whichGroup;
	int whichLevel,whichSensor = -1;

	whichGroup = sensors_Mapping[com1_InputCounter/2];

	whichLevel = (com1_InputCounter%2 == 0) ? 0 : 8;	//1 : 0

	if(c & S1_MASK){
		whichSensor = 1;				
	}
	if(c & S2_MASK){
		whichSensor = 2;				
	}
	if(c & S3_MASK){
		whichSensor = 3;				
	}
	if(c & S4_MASK){
		whichSensor = 4;				
	}
	if(c & S5_MASK){
		whichSensor = 5;				
	}
	if(c & S6_MASK){
		whichSensor = 6;				
	}
	if(c & S7_MASK){
		whichSensor = 7;				
	}
	if(c & S8_MASK){
		whichSensor = 8;				
	}	

	if(whichSensor != -1 && lastSensor != (whichGroup+whichLevel+whichSensor)){

		sensors_Buffer[sensors_Buffer_Index] = whichGroup*100 + whichLevel+whichSensor;	//6
		printSensorsStatus();
		sensors_Buffer_Index = (sensors_Buffer_Index + 1) % 6;	

		lastSensor = whichGroup+whichLevel+whichSensor;	
	}

	com1_InputCounter++;

	if(com1_InputCounter == 10){
		com1_InputCounter = 0;
	}
	return 0;	
}

int bwputc( int channel, char c ) {
	switch( channel ) {
	case COM1:
		com1_OutputBuffer[com1_OutputNextFreeSpot] = c;
		com1_OutputNextFreeSpot = (com1_OutputNextFreeSpot+1) % MAX_BUFFER_SIZE;
		break;
	case COM2:
		com2_OutputBuffer[com2_OutputNextFreeSpot] = c;
		com2_OutputNextFreeSpot = (com2_OutputNextFreeSpot+1) % MAX_BUFFER_SIZE;
		break;
	}

	return 0;
}

int bwsetfifo( int channel, int state ) {
	int *line, buf;
	switch( channel ) {
	case COM1:
		line = (int *)( UART1_BASE + UART_LCRH_OFFSET );
	        break;
	case COM2:
	        line = (int *)( UART2_BASE + UART_LCRH_OFFSET );
	        break;
	default:
	        return -1;
	        break;
	}
	buf = *line;
	buf = state ? buf | FEN_MASK : buf & ~FEN_MASK;
	*line = buf;
	return 0;
}

int bwsetspeed( int channel, int speed ) {
	int *high, *low;
	switch( channel ) {
	case COM1:
		high = (int *)( UART1_BASE + UART_LCRM_OFFSET );
		low = (int *)( UART1_BASE + UART_LCRL_OFFSET );
	        break;
	case COM2:
		high = (int *)( UART2_BASE + UART_LCRM_OFFSET );
		low = (int *)( UART2_BASE + UART_LCRL_OFFSET );
	        break;
	default:
	        return -1;
	        break;
	}
	switch( speed ) {
	case 115200:
		*high = 0x0;
		*low = 0x3;
		return 0;
	case 2400:
		*high = 0x0;
		*low = 0xbf;
		return 0;
	default:
		return -1;
	}
}


char c2x( char ch ) {
	if ( (ch <= 9) ) return '0' + ch;
	return 'a' + ch - 10;
}

int bwputx( int channel, char c ) {
	char chh, chl;

	chh = c2x( c / 16 );
	chl = c2x( c % 16 );
	bwputc( channel, chh );
	return bwputc( channel, chl );
}

int bwputr( int channel, unsigned int reg ) {
	int byte;
	char *ch = (char *) &reg;

	for( byte = 3; byte >= 0; byte-- ) bwputx( channel, ch[byte] );
	return bwputc( channel, ' ' );
}

int bwputstr( int channel, char *str ) {
	while( *str ) {
		if( bwputc( channel, *str ) < 0 ) return -1;
		str++;
	}
	return 0;
}

void bwputw( int channel, int n, char fc, char *bf ) {
	char ch;
	char *p = bf;

	while( *p++ && n > 0 ) n--;
	while( n-- > 0 ) bwputc( channel, fc );
	while( ( ch = *bf++ ) ) bwputc( channel, ch );
}

int bwa2d( char ch ) {
	if( ch >= '0' && ch <= '9' ) return ch - '0';
	if( ch >= 'a' && ch <= 'f' ) return ch - 'a' + 10;
	if( ch >= 'A' && ch <= 'F' ) return ch - 'A' + 10;
	return -1;
}

char bwa2i( char ch, char **src, int base, int *nump ) {
	int num, digit;
	char *p;

	p = *src; num = 0;
	while( ( digit = bwa2d( ch ) ) >= 0 ) {
		if ( digit > base ) break;
		num = num*base + digit;
		ch = *p++;
	}
	*src = p; *nump = num;
	return ch;
}

void bwui2a( unsigned int num, unsigned int base, char *bf ) {
	int n = 0;
	int dgt;
	unsigned int d = 1;
	
	while( (num / d) >= base ) d *= base;
	while( d != 0 ) {
		dgt = num / d;
		num %= d;
		d /= base;
		if( n || dgt > 0 || d == 0 ) {
			*bf++ = dgt + ( dgt < 10 ? '0' : 'a' - 10 );
			++n;
		}
	}
	*bf = 0;
}

void bwi2a( int num, char *bf ) {
	if( num < 0 ) {
		num = -num;
		*bf++ = '-';
	}
	bwui2a( num, 10, bf );
}

void bwformat ( int channel, char *fmt, va_list va ) {
	char bf[12];
	char ch, lz;
	int w;

	
	while ( ( ch = *(fmt++) ) ) {
		if ( ch != '%' )
			bwputc( channel, ch );
		else {
			lz = 0; w = 0;
			ch = *(fmt++);
			switch ( ch ) {
			case '0':
				lz = 1; ch = *(fmt++);
				break;
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				ch = bwa2i( ch, &fmt, 10, &w );
				break;
			}
			switch( ch ) {
			case 0: return;
			case 'c':
				bwputc( channel, va_arg( va, char ) );
				break;
			case 's':
				bwputw( channel, w, 0, va_arg( va, char* ) );
				break;
			case 'u':
				bwui2a( va_arg( va, unsigned int ), 10, bf );
				bwputw( channel, w, lz, bf );
				break;
			case 'd':
				bwi2a( va_arg( va, int ), bf );
				bwputw( channel, w, lz, bf );
				break;
			case 'x':
				bwui2a( va_arg( va, unsigned int ), 16, bf );
				bwputw( channel, w, lz, bf );
				break;
			case '%':
				bwputc( channel, ch );
				break;
			}
		}
	}
}

void myprintf( int channel, char *fmt, ... ) {
        va_list va;

        va_start(va,fmt);
        bwformat( channel, fmt, va );
        va_end(va);
}

