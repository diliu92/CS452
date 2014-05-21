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
Receieve(int* Tid, void* msg, int msglen);

int 
Reply(int Tid, void* reply, int replylen);



#endif
