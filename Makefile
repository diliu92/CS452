#
# Makefile for busy-wait IO tests
#
XCC     = gcc
AS	= as
LD      = ld

OUT_DIR		= bin
SRC_KERN_DIR	= src/kernel
SRC_USER_DIR	= src/user
SRC_COMMON_DIR	= src/common

CFLAGS  = -c -fPIC -Wall -I./include -mcpu=arm920t -msoft-float
# -g: include hooks for gdb
# -c: only compile
# -mcpu=arm920t: generate code for the 920t architecture
# -fpic: emit position-independent code
# -Wall: report all warnings

ASFLAGS	= -mcpu=arm920t -mapcs-32
# -mapcs: always generate a complete stack frame

LDFLAGS = -init main -Map $(OUT_DIR)/main.map -N  -T orex.ld -L/u/wbcowan/gnuarm-4.0.2/lib/gcc/arm-elf/4.0.2

OBJS = 	$(OUT_DIR)/kernel.o		\
	$(OUT_DIR)/syscall_kern.o	\
	$(OUT_DIR)/bwio.o	

all: kernel.elf

kernel.elf: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) -lgcc		

$(OUT_DIR)/kernel.o: $(OUT_DIR)/kernel.s
	$(AS) 	-o $(OUT_DIR)/kernel.o 	$(ASFLAGS) $(OUT_DIR)/kernel.s
$(OUT_DIR)/kernel.s:
	$(XCC) 	-o $(OUT_DIR)/kernel.s 	-S $(CFLAGS) $(SRC_KERN_DIR)/kernel.c

$(OUT_DIR)/syscall_kern.o: $(OUT_DIR)/syscall_kern.s
	$(AS) 	-o $(OUT_DIR)/syscall_kern.o 	$(ASFLAGS) $(OUT_DIR)/syscall_kern.s
$(OUT_DIR)/syscall_kern.s:
	$(XCC) 	-o $(OUT_DIR)/syscall_kern.s 	-S $(CFLAGS) $(SRC_KERN_DIR)/syscall_kern.c


$(OUT_DIR)/bwio.o: $(OUT_DIR)/bwio.s
	$(AS)	-o $(OUT_DIR)/bwio.o 	$(ASFLAGS) $(OUT_DIR)/bwio.s
$(OUT_DIR)/bwio.s: 
	$(XCC) 	-o $(OUT_DIR)/bwio.s 	-S $(CFLAGS) $(SRC_COMMON_DIR)/bwio.c

clean:
	-rm -f $(OUT_DIR)/main.elf $(OUT_DIR)/*.s $(OUT_DIR)/*.o $(OUT_DIR)/main.map
