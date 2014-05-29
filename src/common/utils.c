

void*
memcpy(const char* src1, char* dest1, int size){
	long long* src = (long long*) src1;
	long long* dest = (long long*) dest1;
	
	int longlong_size 	= size / sizeof(long long);
	int char_size 		= size % sizeof(long long);
	
	while(longlong_size--)
		*dest++ = *src++;
	
	char* char_src = (char*) src;
	char* char_dest = (char*) dest;
	
	while(char_size--)
		*char_dest++ = *char_src++;
	
	return dest;
	
	
	/*
	while(size--)
		*dest++ = *src++;
	return dest;
	*/
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
		str2++;
	}
	return 0;
}
