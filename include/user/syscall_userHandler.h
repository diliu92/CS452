#ifndef __SYSCALLUSERHANDLER_H__
#define __SYSCALLUSERHANDLER_H__


int 
Create(int priority, void (*code)());

int 
MyTid();

int 
MyParentTid();

void 
Pass();

void 
Exit();


int 
Send(int Tid, void* msg, int msglen, void* reply, int replylen);

int 
Receive(int* Tid, void* msg, int msglen);

int 
Reply(int Tid, void* reply, int replylen);

int
RegisterAs(char* name);

int
WhoIs(char* name);


int 
AwaitEvent(int eventid);

int 
Delay(int ticks);

int 
DelayUntil(int ticks);

int
Time();

int 
putc(int channel, char c);

char 
getc(int channel);


#endif
