#ifndef __SYSCALLKERNHANDLER_H__
#define __SYSCALLKERNHANDLER_H__

#include <kernel/kernGlobal.h>
#include <common/syscall_information.h>

int 
syscall_kernHandler(kernGlobal* kernelData, syscallRequest* req);

#endif
