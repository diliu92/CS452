#ifndef __SYSCALLKERN_H__
#define __SYSCALLKERN_H__


#include <kernel/kernGlobal.h>


#define SYSCALL_CREATE 			1
#define SYSCALL_MY_TID			2
#define SYSCALL_MY_PARENT_TID	3
#define SYSCALL_PASS			4
#define SYSCALL_EXIT			5
#define SYSCALL_SEND			6
#define SYSCALL_RECEIVE			7
#define SYSCALL_REPLY			8


/*
typedef struct requestMessage{

}








int CreateTask(int, void (*)());



struct task* getNextTask(struct kernelGlobal*);

void pushQueue(struct kernelGlobal* kernelData, int qIdx, struct task* tsk);



*/


#endif
