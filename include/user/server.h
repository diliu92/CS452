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
}

typedef struct nameServerTable{
	nameServerEntry entries;
	int entryStatus[MAX_TASK];
}nameServerTable;

void
nameServer();



/*
 * clockServer Related 
 */
#define CLOCKSERVER		2 
void
clockServer();

/*
 * serialServer Related 
 */
#define SERIALSERVER	3 
void
serialServer();


#endif
