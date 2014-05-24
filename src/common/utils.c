


void*
memcpy(char* src, char* dest, int size){
	while(size--)
		*dest++ = *src++;
	
	return dest;
}
