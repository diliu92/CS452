

void*
memcpy(const char* src1, char* dest1, int size){
	long long* long_src = (long long*) src1;
	long long* long_dest = (long long*) dest1;	
	int longlong_size 	= size / sizeof(long long);	
	while(longlong_size--)
		*long_dest++ = *long_src++;
	
	int* int_src = (int*) long_dest;
	int* int_dest = (int*) long_src;	
	int int_size = size%sizeof(long long) / sizeof(int);	
	while(int_size--)
		*int_dest++ = *int_src++;
	
	char* char_src = (char*) int_src;
	char* char_dest = (char*) int_dest;
	int char_size = size%sizeof(int);	
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
