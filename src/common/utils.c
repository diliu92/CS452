


void*
memcpy(const char* src, char* dest, int size){
	while(size--)
		*dest++ = *src++;
	return dest;
}

int
strlen(const char* str){	
	char* s;	
	for (s = str; *s; ++s);	
	return s - str;	
}
