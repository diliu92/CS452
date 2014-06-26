#ifndef __UTILS_H__
#define __UTILS_H__


#define cyan "\033[36m"
#define magenta "\033[35m"
#define blue "\033[34m"
#define yellow "\033[33m"
#define green "\033[32m"
#define red "\033[31m"

#define clearLine "\033[K"
#define clearScreen "\033[2J"
#define resetColor "\033[0m"

#define tab "\033[4C"

#define save "\033[s"
#define restore "\033[u"

#define NULL (void*)0

#define TRUE	0xFFFFFFFF
#define FALSE	0x00000000


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
