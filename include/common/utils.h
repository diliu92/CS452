#ifndef __UTILS_H__
#define __UTILS_H__


#define NULL (void*)0


typedef struct Queue{
	void* head;	
	void* tail;		
}Queue;

void*
memcpy(const char* src, char* dest, int size);

int
strlen(const char* str);

int
strcomp(const char* str1, const char* str2);



#endif
