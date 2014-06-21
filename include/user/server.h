#ifndef __SERVER_H__
#define __SERVER_H__

#include <kernel/kernGlobal.h>


#define TYPE_NOTIFIER		0
#define TYPE_NOTIFIER_RECV	1
#define TYPE_NOTIFIER_SEND	2
#define TYPE_CLIENT			3

/*
 * NameServer Related 
 */ 
#define NAMESERVER_TID	1

#define MAX_NAME_SIZE 32 
#define MAX_NAME_ENTRY_PER_TID	4
 
typedef struct nameEntry{
	int size;
	char data[MAX_NAME_SIZE];
}nameEntry; 
 
typedef struct nameServerEntry{
	int tid;
	int nextSpot;
	nameEntry names[4];
}nameServerEntry;

typedef struct nameServerTable{
	nameServerEntry entries[MAX_TASK];
	int entryStatus[MAX_TASK];
}nameServerTable;

void
nameServer();


/*
 * clockServer Related 
 */
void
clockServer();


/*
 * serialServer Related 
 */
// #define SERIALSERVER_TID	3 
// void
// serialServer();


void
UART2_Server();

void
UART1_Server();

#endif
