/*
 * io.h
 */

typedef char *va_list;

#define __va_argsiz(t)	\
		(((sizeof(t) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))

#define va_start(ap, pN) ((ap) = ((va_list) __builtin_next_arg(pN)))

#define va_end(ap)	((void)0)

#define va_arg(ap, t)	\
		 (((ap) = (ap) + __va_argsiz(t)), *((t*) (void*) ((ap) - __va_argsiz(t))))

#define COM1	0
#define COM2	1

#define ON	1
#define	OFF	0

#define straight 33
#define curve 	34

#define MAX_BUFFER_SIZE 1024
#define COMMAND_BUFFER_SIZE 128

#define S1_MASK 	0x80
#define S2_MASK 	0x40
#define S3_MASK 	0x20
#define S4_MASK 	0x10
#define S5_MASK 	0x08
#define S6_MASK 	0x04
#define S7_MASK 	0x02
#define S8_MASK 	0x01

#define	TIMER3_LOAD	(unsigned int *)(TIMER3_BASE + LDR_OFFSET)
#define	TIMER3_VALUE	(unsigned int *)(TIMER3_BASE + VAL_OFFSET)
#define	TIMER3_CONTROL	(unsigned int *)(TIMER3_BASE + CRTL_OFFSET)

void ioInit(char*,char*,char*,char*,int*,int*,int*);
void trainsSpeedandSwitchInit();

int doesChannelNeedToPut(int channel);

int needToQuerySensors();

int isChannelRdyToPut(int channel);
int isChannelRdyToGet(int channel);

void putc(int channel);
unsigned char getc(int channel);

int processUserCommands(char c);
int processSensorResult(char c);

int bwputc( int channel, char c );	//changed

int bwsetfifo( int channel, int state );

int bwsetspeed( int channel, int speed );

int bwputx( int channel, char c );

int bwputstr( int channel, char *str );

int bwputr( int channel, unsigned int reg );

void bwputw( int channel, int n, char fc, char *bf );

void myprintf( int channel, char *format, ... );
