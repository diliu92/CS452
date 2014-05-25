#include <user.h>

static syscallRequest* 
putReqInR0(syscallRequest* req){
	return req;
}

int 
Create(int priority, void (*code)()){
	syscallRequest_Create req;
	req.syscall_uid = SYSCALL_CREATE;
	req.priority = priority;
	req.code = code;
	
	putReqInR0(&req);
	//to be fixed, hardware-interrupt
	asm("swi");

	return req.retval;
}

int 
MyTid(){
	syscallRequest req;
	req.syscall_uid = SYSCALL_MY_TID;
	
	putReqInR0(&req);
	asm("swi");

	return req.retval;
}

int 
MyParentTid(){
	syscallRequest req;
	req.syscall_uid = SYSCALL_MY_PARENT_TID;
	
	putReqInR0(&req);
	asm("swi");

	return req.retval;	
}

void 
Pass(){
	syscallRequest req;
	req.syscall_uid = SYSCALL_PASS;
	
	putReqInR0(&req);
	asm("swi");
}

void 
Exit(){
	syscallRequest req;
	req.syscall_uid = SYSCALL_EXIT;
	
	putReqInR0(&req);
	asm("swi");
}


int 
Send(int Tid, void* msg, int msglen, void* reply, int replylen){
	syscallRequest_Send req;
	req.syscall_uid = SYSCALL_SEND;
	req.Tid = Tid;
	req.msg = msg;
	req.msglen = msglen;
	req.reply = reply;
	req.replylen = replylen;
	
	putReqInR0(&req);
	asm("swi");
	
	return req.retval;	
}

int 
Receieve(int* Tid, void* msg, int msglen){
	syscallRequest_Receive req;
	req.syscall_uid = SYSCALL_RECEIVE;
	req.Tid = Tid;
	req.msg = msg;
	req.msglen = msglen;
	
	putReqInR0(&req);
	asm("swi");
	
	return req.retval;	
}

int 
Reply(int Tid, void* reply, int replylen){
	syscallRequest_Reply req;
	req.syscall_uid = SYSCALL_REPLY;
	req.Tid = Tid;
	req.reply = reply;
	req.replylen = replylen;
	
	putReqInR0(&req);
	asm("swi");
	
	return req.retval;	
}

int
RegisterAs(char* name){
	syscallRequest_NameServer req;
	req.syscall_uid = SYSCALL_REGISTERAS;
	req.tid = MyTid();
	req.name = name;
	req.size = strLen(name);
	
	int retval;
	
	Send(NAMESERVER_TID, &req, sizeof(syscallRequest_NameServer), &retval, sizeof(int));
	
	return retval; 	
}

int
WhoIs(char* name){
	syscallRequest_NameServer req;
	req.syscall_uid = SYSCALL_WHOIS;
	req.name = name;
	req.size = strLen(name);
		
	Send(NAMESERVER_TID, &req, sizeof(syscallRequest_NameServer), &(req.tid), sizeof(int));
	
	return req.tid; 	
}

