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

OBJS = 	$(OUT_DIR)/kernel.o			\
	$(OUT_DIR)/kernInit.o			\
	$(OUT_DIR)/syscall_kern.o		\
	$(OUT_DIR)/syscall_kernHandler.o	\
	$(OUT_DIR)/contextSwitch.o		\
	$(OUT_DIR)/syscall_userHandler.o	\
	$(OUT_DIR)/first_user_task.o		\
	$(OUT_DIR)/bwio.o			


all: kernel.elf

kernel.elf: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) -lgcc		

$(OUT_DIR)/kernel.o: $(SRC_KERN_DIR)/kernel.s
	$(AS) 	-o $(OUT_DIR)/kernel.o 	$(ASFLAGS) $(SRC_KERN_DIR)/kernel.s
$(SRC_KERN_DIR)/kernel.s:
	$(XCC) 	-o $(SRC_KERN_DIR)/kernel.s 	-S $(CFLAGS) $(SRC_KERN_DIR)/kernel.c

$(OUT_DIR)/kernInit.o: $(OUT_DIR)/kernInit.s
	$(AS) 	-o $(OUT_DIR)/kernInit.o 	$(ASFLAGS) $(OUT_DIR)/kernInit.s
$(OUT_DIR)/kernInit.s:
	$(XCC) 	-o $(OUT_DIR)/kernInit.s 	-S $(CFLAGS) $(SRC_KERN_DIR)/kernInit.c

$(OUT_DIR)/syscall_kern.o: $(OUT_DIR)/syscall_kern.s
	$(AS) 	-o $(OUT_DIR)/syscall_kern.o 	$(ASFLAGS) $(OUT_DIR)/syscall_kern.s
$(OUT_DIR)/syscall_kern.s:
	$(XCC) 	-o $(OUT_DIR)/syscall_kern.s 	-S $(CFLAGS) $(SRC_KERN_DIR)/syscall_kern.c

$(OUT_DIR)/syscall_kernHandler.o: $(OUT_DIR)/syscall_kernHandler.s
	$(AS) 	-o $(OUT_DIR)/syscall_kernHandler.o 	$(ASFLAGS) $(OUT_DIR)/syscall_kernHandler.s
$(OUT_DIR)/syscall_kernHandler.s:
	$(XCC) 	-o $(OUT_DIR)/syscall_kernHandler.s 	-S $(CFLAGS) $(SRC_KERN_DIR)/syscall_kernHandler.c

$(OUT_DIR)/contextSwitch.o: 
	$(AS)	-o $(OUT_DIR)/contextSwitch.o 	$(ASFLAGS) $(SRC_KERN_DIR)/contextSwitch.s

$(OUT_DIR)/syscall_userHandler.o: $(OUT_DIR)/syscall_userHandler.s
	$(AS) 	-o $(OUT_DIR)/syscall_userHandler.o 	$(ASFLAGS) $(OUT_DIR)/syscall_userHandler.s
$(OUT_DIR)/syscall_userHandler.s:
	$(XCC) 	-o $(OUT_DIR)/syscall_userHandler.s 	-S $(CFLAGS) $(SRC_USER_DIR)/syscall_userHandler.c

$(OUT_DIR)/first_user_task.o: $(OUT_DIR)/first_user_task.s
	$(AS) 	-o $(OUT_DIR)/first_user_task.o 	$(ASFLAGS) $(OUT_DIR)/first_user_task.s
$(OUT_DIR)/first_user_task.s:
	$(XCC) 	-o $(OUT_DIR)/first_user_task.s 	-S $(CFLAGS) $(SRC_USER_DIR)/first_user_task.c

$(OUT_DIR)/bwio.o: $(OUT_DIR)/bwio.s
	$(AS)	-o $(OUT_DIR)/bwio.o 	$(ASFLAGS) $(OUT_DIR)/bwio.s
$(OUT_DIR)/bwio.s: 
	$(XCC) 	-o $(OUT_DIR)/bwio.s 	-S $(CFLAGS) $(SRC_COMMON_DIR)/bwio.c

clean:
	-rm -f kernel.elf $(OUT_DIR)/*.s $(OUT_DIR)/*.o $(OUT_DIR)/main.map
