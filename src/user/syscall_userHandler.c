
#include <user.h>


int 
Create(int priority, void (*code)()){
	syscallRequest_Create req;
	req.syscall_uid = SYSCALL_CREATE;
	req.priority = priority;
	req.code = code;
	
	//assemblyCode(&req);
	
	return req.retval;
}

int 
MyTid(){
	syscallRequest req;
	req.syscall_uid = SYSCALL_MY_TID;
	
	//assemblyCode(&req);
	
	return req.retval;
}

int 
MyParentTid(){
	syscallRequest req;
	req.syscall_uid = SYSCALL_MY_PARENT_TID;
	
	//assemblyCode(&req);
	
	return req.retval;	
}

void 
Pass(){
	syscallRequest req;
	req.syscall_uid = SYSCALL_PASS;
	
	//assemblyCode(&req);
	
}

void 
Exit(){
	syscallRequest req;
	req.syscall_uid = SYSCALL_EXIT;
	
	//assemblyCode(&req);
	
}


/*
int 
Send(int Tid, void* msg, int msglen, void* reply, int replylen);

int 
Receieve(int* Tid, void* msg, int msglen);

int 
Reply(int Tid, void* reply, int replylen);
*/
