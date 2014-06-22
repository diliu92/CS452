/*
 * modified bwio.h
 */

#define PUT 0
#define GET 0

int a2d(char ch);

int putx( int channel, char c );

int putstr( int channel, char *str );

int putr( int channel, unsigned int reg );

void putw( int channel, int n, char fc, char *bf );

void sprintf( int channel, char *format, ... );
