


void*
memcpy(const char* src, char* dest, int size){
	while(size--)
		*dest++ = *src++;
	return dest;
}

int
strLen(const char* str){	
	char* s;	
	for (s = str; *s; s++);	
	return s - str;	
}

int
strEnq(const char* str1, const char* str2){
	while(*str1 == *str2){
		if(*str1 == '\0')
			return 1;
		str1++;
		str2++
	}
	return 0;
}
