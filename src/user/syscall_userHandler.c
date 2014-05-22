#include <user.h>

static syscallRequest * 
putReqInR0( syscallRequest *req ){
	return req;
}

int 
Create(int priority, void (*code)()){
	syscallRequest_Create req;
	req.syscall_uid = SYSCALL_CREATE;
	req.priority = priority;
	req.code = code;
	putReqInR0(&req);
	asm("swi");
	bwprintf( COM2, "after swi.\n\r" );
	return req.retval;
}

int 
MyTid(){
	syscallRequest req;
	req.syscall_uid = SYSCALL_MY_TID;
	
	putReqInR0(&req);
	asm("swi");
	bwprintf( COM2, "after swi.\n\r" );
	return req.retval;
}

int 
MyParentTid(){
	syscallRequest req;
	req.syscall_uid = SYSCALL_MY_PARENT_TID;
	
	putReqInR0(&req);
	asm("swi");
	bwprintf( COM2, "after swi.\n\r" );
	return req.retval;	
}

void 
Pass(){
	syscallRequest req;
	req.syscall_uid = SYSCALL_PASS;
	
	putReqInR0(&req);
	asm("swi");
	bwprintf( COM2, "after swi.\n\r" );
}

void 
Exit(){
	syscallRequest req;
	req.syscall_uid = SYSCALL_EXIT;
	
	putReqInR0(&req);
	asm("swi");
	bwprintf( COM2, "after swi.\n\r" );
}


/*
int 
Send(int Tid, void* msg, int msglen, void* reply, int replylen);

int 
Receieve(int* Tid, void* msg, int msglen);

int 
Reply(int Tid, void* reply, int replylen);
*/
