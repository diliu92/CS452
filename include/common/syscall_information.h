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



#endif
