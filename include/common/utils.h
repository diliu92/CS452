#ifndef __UTILS_H__
#define __UTILS_H__


#define NULL (void*)0

typedef struct Queue{
	void* head;	
	void* tail;		
}Queue;

void*
memcpy(const char* src1, char* dest1, int size);

int
strLen(const char* str);	//doesn't include null terminator

int
strEnq(const char* str1, const char* str2);



#endif
