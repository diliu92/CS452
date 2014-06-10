#include <user.h>

static syscallRequest* 
call_swi(syscallRequest* req){
	asm("swi");
	return req;
}

int 
Create(int priority, void (*code)()){
	syscallRequest_Create req;
	req.syscall_uid = SYSCALL_CREATE;
	req.priority = priority;
	req.code = code;
	
	call_swi(&req);

	return req.retval;
}

int 
MyTid(){
	syscallRequest req;
	req.syscall_uid = SYSCALL_MY_TID;
	
	call_swi(&req);

	return req.retval;
}

int 
MyParentTid(){
	syscallRequest req;
	req.syscall_uid = SYSCALL_MY_PARENT_TID;
	
	call_swi(&req);

	return req.retval;	
}

void 
Pass(){
	syscallRequest req;
	req.syscall_uid = SYSCALL_PASS;
	
	call_swi(&req);

}

void 
Exit(){
	syscallRequest req;
	req.syscall_uid = SYSCALL_EXIT;
	
	call_swi(&req);

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
	
	call_swi(&req);
	
	return req.retval;	
}

int 
Receive(int* Tid, void* msg, int msglen){
	syscallRequest_Receive req;
	req.syscall_uid = SYSCALL_RECEIVE;
	req.Tid = Tid;
	req.msg = msg;
	req.msglen = msglen;
	
	call_swi(&req);
	
	return req.retval;	
}

int 
Reply(int Tid, void* reply, int replylen){
	syscallRequest_Reply req;
	req.syscall_uid = SYSCALL_REPLY;
	req.Tid = Tid;
	req.reply = reply;
	req.replylen = replylen;
	
	call_swi(&req);
	
	return req.retval;	
}

int
RegisterAs(char* name){
	syscallRequest_NameServer req;
	req.syscall_uid = SYSCALL_REGISTERAS;
	req.tid = MyTid();
	req.name = name;
	req.size = strLen(name);
	
	Send(NAMESERVER_TID, &req, sizeof(syscallRequest_NameServer), &(req.retval), sizeof(int));
	
	return req.retval; 	
}

int
WhoIs(char* name){
	syscallRequest_NameServer req;
	req.syscall_uid = SYSCALL_WHOIS;
	req.name = name;
	req.size = strLen(name);
		
	Send(NAMESERVER_TID, &req, sizeof(syscallRequest_NameServer), &(req.retval), sizeof(int));

	return req.retval; 	
}

int 
AwaitEvent(int eventid){
	syscallRequest_Await req;
	req.syscall_uid = SYSCALL_AWAIT;
	req.eventid = eventid;
	
	call_swi(&req);

	return req.retval;	
}

int 
Delay(int ticks){
	syscallRequest_ClockServer req;
	req.syscall_uid = SYSCALL_DELAY;
	req.tid = MyTid();
	req.type = TYPE_CLIENT;
	req.ticks = ticks;

	int clockServerTid = WhoIs("Clock Server");
	Send(clockServerTid, &req, sizeof(syscallRequest_ClockServer), &(req.retval), sizeof(int));

	return req.retval;
}

int 
DelayUntil(int ticks){
	syscallRequest_ClockServer req;
	req.syscall_uid = SYSCALL_DELAYUNTIL;
	req.tid = MyTid();
	req.type = TYPE_CLIENT;
	req.ticks = ticks;

	int clockServerTid = WhoIs("Clock Server");
	Send(clockServerTid, &req, sizeof(syscallRequest_ClockServer), &(req.retval), sizeof(int));

	return req.retval;
}

int
Time(){
	syscallRequest_ClockServer req;
	req.syscall_uid = SYSCALL_TIME;
	req.tid = MyTid();
	req.type = TYPE_CLIENT;

	int clockServerTid = 2; //WhoIs("Clock Server");
	Send(clockServerTid, &req, sizeof(syscallRequest_ClockServer), &(req.retval), sizeof(int));

	return req.retval;
}
