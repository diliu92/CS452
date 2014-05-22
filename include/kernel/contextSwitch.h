#ifndef __CONTEXTSWITCH_H__
#define __CONTEXTSWITCH_H__

#include <kernel.h>

void kerent();
syscallRequest * kerxit( task *active, syscallRequest **req );

#endif
