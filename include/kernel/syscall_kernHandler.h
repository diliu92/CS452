#ifndef __SYSCALLKERNHANDLER_H__
#define __SYSCALLKERNHANDLER_H__



#define SYSCALL_CREATE 			1
#define SYSCALL_MY_TID			2
#define SYSCALL_MY_PARENT_TID	3
#define SYSCALL_PASS			4
#define SYSCALL_EXIT			5
#define SYSCALL_SEND			6
#define SYSCALL_RECEIVE			7
#define SYSCALL_REPLY			8


 
/*
 * for syscall functions that have no arguments
 */ 
typedef struct requestMessage{
	int syscall_uid;	
}requestMessage;


typedef struct requestMessage_Create{
	int syscall_uid;
	int priority;
	void (*code)();
}requestMessage_Create;

/*
typedef struct requestMessage_Send{
	int syscall_uid;
	int Tid;
	void (*code)();
}requestMessage_Send;

typedef struct requestMessage_Create{
	int syscall_uid;
	int priority;
	void (*code)();
}requestMessage_Create;

typedef struct requestMessage_Create{
	int syscall_uid;
	int priority;
	void (*code)();
}requestMessage_Create;
*/

int syscall_kernHandler(kernGlobal* kernelData, requestMessage* req);

#endif
