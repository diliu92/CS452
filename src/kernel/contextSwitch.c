#include <contextSwitch.h>

void
kerent(){}

void
kerxit( task *active, syscallRequest **req ) {
	kerent( );
}