#ifndef __CONTEXTSWITCH_H__
#define __CONTEXTSWITCH_H__

#include <kernel.h>

void prekerent( syscallRequest *req );
void kerent();
void kerxit( task *active, syscallRequest **req );

#endif
