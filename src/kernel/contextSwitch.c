#include <kernel.h>

//void kerent( syscallRequest *req ){}
void kerent( ){}
void kerxit( task *active, syscallRequest **req ) {
	kerent();
}
