#ifndef __SYSCALLINFORMATION_H__
#define __SYSCALLINFORMATION_H__



#define SYSCALL_CREATE 			1
#define SYSCALL_MY_TID			2
#define SYSCALL_MY_PARENT_TID	3
#define SYSCALL_PASS			4
#define SYSCALL_EXIT			5
#define SYSCALL_SEND			6
#define SYSCALL_RECEIVE			7
#define SYSCALL_REPLY			8
#define SYSCALL_AWAIT 			9	

#define SYSCALL_REGISTERAS		10
#define SYSCALL_WHOIS			11

#define SYSCALL_DELAY			12
#define	SYSCALL_DELAYUNTIL		13
#define SYSCALL_TIME			14

#define SYSCALL_GETC			15
#define SYSCALL_PUTC			16

#define TIMER_EVENT			0
#define UART1_SEND_EVENT	1
#define UART1_RECV_EVENT	2
#define UART2_SEND_EVENT	3
#define UART2_RECV_EVENT	4

#define CLOCK_SERVER_TID			2
#define CLOCK_NOTIFIER_TID			3

#define UART2_SERIAL_SERVER_TID		4
#define UART2_SEND_NOTIFIER_TID		5
#define UART2_RECV_NOTIFIER_TID		6

#define UART1_SERIAL_SERVER_TID		7
#define UART1_SEND_NOTIFIER_TID		8
#define UART1_RECV_NOTIFIER_TID		9

/*
 * for syscall functions that have no arguments
 */ 
typedef struct syscallRequest{
	int syscall_uid;
	int retval;
}syscallRequest;


typedef struct syscallRequest_Create{
	int syscall_uid;
	int	retval;
	
	int priority;
	void (*code)();
}syscallRequest_Create;

typedef struct syscallRequest_Send{
	int syscall_uid;
	int	retval;
	
	int Tid;
	void* msg;
	int msglen;
	void* reply;
	int replylen;
}syscallRequest_Send;

typedef struct syscallRequest_Receive{
	int syscall_uid;
	int	retval;
	
	int* Tid;
	void* msg;
	int msglen;
}syscallRequest_Receive;

typedef struct syscallRequest_Reply{
	int syscall_uid;
	int	retval;
	
	int Tid;
	void* reply;
	int replylen;
}syscallRequest_Reply;

typedef struct syscallRequest_Await{
	int syscall_uid;
	int	retval;
	
	int eventid;
}syscallRequest_Await;

typedef struct syscallRequest_NameServer{
	int syscall_uid;
	int retval;

	int tid;
	char* name;
	int size;
}syscallRequest_NameServer;

typedef struct syscallRequest_ClockServer{
	int syscall_uid;
	int retval;

	int tid;
	int ticks;
	int type;
}syscallRequest_ClockServer;

typedef struct syscallRequest_UARTServer{
	int syscall_uid;
	int retval;

	int tid;
	char data;
	int type;
}syscallRequest_UARTServer;

#endif
