#ifndef __CONTEXTSWITCH_H__
#define __CONTEXTSWITCH_H__

#include <kernel/kernGlobal.h>

#include <common/syscall_information.h>

void 
kerent();

syscallRequest* 
kerxit(task* active);

#endif
