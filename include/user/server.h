#ifndef __SERVER_H__
#define __SERVER_H__

#include <kernel/kernGlobal.h>

/*
 * NameServer Related 
 */ 
#define NAMESERVER_TID	1

#define MAX_NAME_SIZE 32 
 
typedef struct name{
	int size;
	char data[MAX_NAME_SIZE];
}name; 
 
typedef struct nameServerEntry{
	int tid;
	int nextSpot;
	name names[4];
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
#define CLOCKSERVER_TID		2 
void
clockServer();

/*
 * serialServer Related 
 */
#define SERIALSERVER_TID	3 
void
serialServer();


#endif
