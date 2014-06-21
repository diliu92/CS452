#ifndef __KERNINIT_H__
#define __KERNINIT_H__

#include <kernel/kernGlobal.h>

#define TIMER_INTERRUPT 51
#define UART1_INTERRUPT 52
#define UART2_INTERRUPT 54

void
Init(kernGlobal* kernelData);

#endif
