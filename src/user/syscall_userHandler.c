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
AwaitEvent(int eventid, char* event, int eventlen){
	syscallRequest_Await req;
	req.syscall_uid = SYSCALL_AWAIT;

	req.eventid = eventid;
	req.event = event;
	req.eventlen = eventlen;
	
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

	int clockServerTid = CLOCK_SERVER_TID;//WhoIs("Clock Server");
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

	int clockServerTid = CLOCK_SERVER_TID;//WhoIs("Clock Server");
	Send(clockServerTid, &req, sizeof(syscallRequest_ClockServer), &(req.retval), sizeof(int));

	return req.retval;
}

int
Time(){
	syscallRequest_ClockServer req;
	req.syscall_uid = SYSCALL_TIME;
	req.tid = MyTid();
	req.type = TYPE_CLIENT;

	int clockServerTid = CLOCK_SERVER_TID;//WhoIs("Clock Server");
	Send(clockServerTid, &req, sizeof(syscallRequest_ClockServer), &(req.retval), sizeof(int));

	return req.retval;
}

char 
getc(int channel){
	syscallRequest_UARTServer req;
	req.syscall_uid = SYSCALL_GETC;
	req.tid = MyTid();
	req.type = TYPE_CLIENT;

	int uartServerTid;
	switch(channel){
		case COM1:
			uartServerTid = UART1_SERIAL_SERVER_TID;//WhoIs("UART1 Server");
			break;
		case COM2:
			uartServerTid = UART2_SERIAL_SERVER_TID;//WhoIs("UART2 Server");
			break;	
	}
	Send(uartServerTid, &req, sizeof(syscallRequest_UARTServer), &(req.retval), sizeof(int));

	return req.retval;
}

int 
putc(int channel, char c){
	syscallRequest_UARTServer req;
	req.syscall_uid = SYSCALL_PUTC;
	req.tid = MyTid();
	req.type = TYPE_CLIENT;
	req.data = c;

	int uartServerTid;
	switch(channel){
		case COM1:
			uartServerTid = UART1_SERIAL_SERVER_TID;//WhoIs("UART1 Server");
			break;
		case COM2:
			uartServerTid = UART2_SERIAL_SERVER_TID;//WhoIs("UART2 Server");
			break;	
	}
	Send(uartServerTid, &req, sizeof(syscallRequest_UARTServer), &(req.retval), sizeof(int));
	
	return req.retval;
}


void 
ShutDown(){
	syscallRequest req;
	req.syscall_uid = SYSCALL_SHUTDOWN;
	
	call_swi(&req);
}

int 
NeedToShutDown(){
	syscallRequest req;
	req.syscall_uid = SYSCALL_NEEDTOSHUTDOWN;
	
	call_swi(&req);

	return req.retval;
}