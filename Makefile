#
# Makefile for busy-wait IO tests
#
XCC 	= gcc
AS	= as
LD  	= ld

OUT_DIR		= bin
SRC_KERN_DIR	= src/kernel
SRC_USER_DIR	= src/user
SRC_TRAIN_DIR	= src/train
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

KERN_OBJS = 	$(OUT_DIR)/kernel.o			\
		$(OUT_DIR)/kernInit.o			\
		$(OUT_DIR)/syscall_kern.o		\
		$(OUT_DIR)/syscall_kernHandler.o				

USER_OBJS=	$(OUT_DIR)/syscall_userHandler.o	\
		$(OUT_DIR)/first_user_task.o		\
		$(OUT_DIR)/nameServer.o			\
		$(OUT_DIR)/clockServer.o		\
		$(OUT_DIR)/uartServer1.o		\
		$(OUT_DIR)/uartServer2.o		\
		$(OUT_DIR)/processors.o				

COMMON_OBJS=	$(OUT_DIR)/contextSwitch.o		\
		$(OUT_DIR)/bwio.o			\
		$(OUT_DIR)/serialio.o			\
		$(OUT_DIR)/utils.o

TRAIN_OBJS=	$(OUT_DIR)/trackServer.o		\
		$(OUT_DIR)/trackData.o		\
		$(OUT_DIR)/routeServer.o


all: demo1.elf

demo1.elf: $(KERN_OBJS) $(USER_OBJS) $(COMMON_OBJS) $(TRAIN_OBJS) 
	$(LD) $(LDFLAGS) -o $@ $(KERN_OBJS) $(USER_OBJS) $(COMMON_OBJS) $(TRAIN_OBJS) -lgcc	
	
#-------------------------------------kernel related----------------------------------------#
$(OUT_DIR)/kernel.o: $(OUT_DIR)/kernel.s
	$(AS) 	-o $(OUT_DIR)/kernel.o 	$(ASFLAGS) $(OUT_DIR)/kernel.s
$(OUT_DIR)/kernel.s:
	$(XCC) 	-o $(OUT_DIR)/kernel.s 	-S $(CFLAGS) $(SRC_KERN_DIR)/kernel.c

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


#-------------------------------------user related----------------------------------------#
$(OUT_DIR)/syscall_userHandler.o: $(OUT_DIR)/syscall_userHandler.s
	$(AS) 	-o $(OUT_DIR)/syscall_userHandler.o 	$(ASFLAGS) $(OUT_DIR)/syscall_userHandler.s
$(OUT_DIR)/syscall_userHandler.s:
	$(XCC) 	-o $(OUT_DIR)/syscall_userHandler.s 	-S $(CFLAGS) $(SRC_USER_DIR)/syscall_userHandler.c

$(OUT_DIR)/first_user_task.o: $(OUT_DIR)/first_user_task.s
	$(AS) 	-o $(OUT_DIR)/first_user_task.o 	$(ASFLAGS) $(OUT_DIR)/first_user_task.s
$(OUT_DIR)/first_user_task.s:
	$(XCC) 	-o $(OUT_DIR)/first_user_task.s 	-S $(CFLAGS) $(SRC_USER_DIR)/first_user_task.c

$(OUT_DIR)/nameServer.o: $(OUT_DIR)/nameServer.s
	$(AS) 	-o $(OUT_DIR)/nameServer.o 	$(ASFLAGS) $(OUT_DIR)/nameServer.s
$(OUT_DIR)/nameServer.s:
	$(XCC) 	-o $(OUT_DIR)/nameServer.s 	-S $(CFLAGS) $(SRC_USER_DIR)/nameServer.c

$(OUT_DIR)/clockServer.o: $(OUT_DIR)/clockServer.s
	$(AS) 	-o $(OUT_DIR)/clockServer.o 	$(ASFLAGS) $(OUT_DIR)/clockServer.s
$(OUT_DIR)/clockServer.s:
	$(XCC) 	-o $(OUT_DIR)/clockServer.s 	-S $(CFLAGS) $(SRC_USER_DIR)/clockServer.c

$(OUT_DIR)/uartServer1.o: $(OUT_DIR)/uartServer1.s
	$(AS) 	-o $(OUT_DIR)/uartServer1.o 	$(ASFLAGS) $(OUT_DIR)/uartServer1.s
$(OUT_DIR)/uartServer1.s:
	$(XCC) 	-o $(OUT_DIR)/uartServer1.s 	-S $(CFLAGS) $(SRC_USER_DIR)/uartServer1.c

$(OUT_DIR)/uartServer2.o: $(OUT_DIR)/uartServer2.s
	$(AS) 	-o $(OUT_DIR)/uartServer2.o 	$(ASFLAGS) $(OUT_DIR)/uartServer2.s
$(OUT_DIR)/uartServer2.s:
	$(XCC) 	-o $(OUT_DIR)/uartServer2.s 	-S $(CFLAGS) $(SRC_USER_DIR)/uartServer2.c

$(OUT_DIR)/processors.o: $(OUT_DIR)/processors.s
	$(AS) 	-o $(OUT_DIR)/processors.o 	$(ASFLAGS) $(OUT_DIR)/processors.s
$(OUT_DIR)/processors.s:
	$(XCC) 	-o $(OUT_DIR)/processors.s 	-S $(CFLAGS) $(SRC_USER_DIR)/processors.c


#-------------------------------------common related----------------------------------------#
$(OUT_DIR)/contextSwitch.o: 
	$(AS)	-o $(OUT_DIR)/contextSwitch.o 	$(ASFLAGS) $(SRC_COMMON_DIR)/contextSwitch.s

$(OUT_DIR)/bwio.o: $(OUT_DIR)/bwio.s
	$(AS)	-o $(OUT_DIR)/bwio.o 	$(ASFLAGS) $(OUT_DIR)/bwio.s
$(OUT_DIR)/bwio.s: 
	$(XCC) 	-o $(OUT_DIR)/bwio.s 	-S $(CFLAGS) $(SRC_COMMON_DIR)/bwio.c

$(OUT_DIR)/serialio.o: $(OUT_DIR)/serialio.s
	$(AS)	-o $(OUT_DIR)/serialio.o 	$(ASFLAGS) $(OUT_DIR)/serialio.s
$(OUT_DIR)/serialio.s: 
	$(XCC) 	-o $(OUT_DIR)/serialio.s 	-S $(CFLAGS) $(SRC_COMMON_DIR)/serialio.c

$(OUT_DIR)/utils.o: $(OUT_DIR)/utils.s
	$(AS)	-o $(OUT_DIR)/utils.o 	$(ASFLAGS) $(OUT_DIR)/utils.s
$(OUT_DIR)/utils.s: 
	$(XCC) 	-o $(OUT_DIR)/utils.s 	-S $(CFLAGS) $(SRC_COMMON_DIR)/utils.c


#-------------------------------------train related----------------------------------------#
$(OUT_DIR)/trackServer.o: $(OUT_DIR)/trackServer.s
	$(AS) 	-o $(OUT_DIR)/trackServer.o 	$(ASFLAGS) $(OUT_DIR)/trackServer.s
$(OUT_DIR)/trackServer.s:
	$(XCC) 	-o $(OUT_DIR)/trackServer.s 	-S $(CFLAGS) $(SRC_TRAIN_DIR)/trackServer.c

$(OUT_DIR)/trackData.o: $(OUT_DIR)/trackData.s
	$(AS) 	-o $(OUT_DIR)/trackData.o 	$(ASFLAGS) $(OUT_DIR)/trackData.s
$(OUT_DIR)/trackData.s:
	$(XCC) 	-o $(OUT_DIR)/trackData.s 	-S $(CFLAGS) $(SRC_TRAIN_DIR)/trackData.c

$(OUT_DIR)/routeServer.o: $(OUT_DIR)/routeServer.s
	$(AS) 	-o $(OUT_DIR)/routeServer.o 	$(ASFLAGS) $(OUT_DIR)/routeServer.s
$(OUT_DIR)/routeServer.s:
	$(XCC) 	-o $(OUT_DIR)/routeServer.s 	-S $(CFLAGS) $(SRC_TRAIN_DIR)/routeServer.c



clean:
	-rm -f demo1.elf $(OUT_DIR)/*.s $(OUT_DIR)/*.o $(OUT_DIR)/main.map
