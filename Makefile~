#
# Makefile for busy-wait IO tests
#
XCC     = gcc
AS	= as
LD      = ld
OUT_DIR	= bin
SRC_DIR	= src

CFLAGS  = -c -fPIC -Wall -I./include -mcpu=arm920t -msoft-float
# -g: include hooks for gdb
# -c: only compile
# -mcpu=arm920t: generate code for the 920t architecture
# -fpic: emit position-independent code
# -Wall: report all warnings

ASFLAGS	= -mcpu=arm920t -mapcs-32
# -mapcs: always generate a complete stack frame

LDFLAGS = -init main -Map $(OUT_DIR)/main.map -N  -T orex.ld -L/u/wbcowan/gnuarm-4.0.2/lib/gcc/arm-elf/4.0.2

OBJS = 	$(OUT_DIR)/main.o	\
	$(OUT_DIR)/io.o

all: main.elf

main.elf: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) -lgcc		

$(OUT_DIR)/main.o: $(OUT_DIR)/main.s
	$(AS) 	-o $(OUT_DIR)/main.o 	$(ASFLAGS) $(OUT_DIR)/main.s

$(OUT_DIR)/main.s:
	$(XCC) 	-o $(OUT_DIR)/main.s 	-S $(CFLAGS) $(SRC_DIR)/main.c

$(OUT_DIR)/io.o: $(OUT_DIR)/io.s
	$(AS)	-o $(OUT_DIR)/io.o 	$(ASFLAGS) $(OUT_DIR)/io.s

$(OUT_DIR)/io.s: 
	$(XCC) 	-o $(OUT_DIR)/io.s 	-S $(CFLAGS) $(SRC_DIR)/io.c

clean:
	-rm -f $(OUT_DIR)/main.elf $(OUT_DIR)/*.s $(OUT_DIR)/*.o $(OUT_DIR)/main.map
