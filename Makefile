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

all:  $(OUT_DIR)/io.s $(OUT_DIR)/main.s main.elf

$(OUT_DIR)/main.s: $(SRC_DIR)/main.c 
	$(XCC) -S $(CFLAGS) $(SRC_DIR)/main.c

$(OUT_DIR)/main.o: $(OUT_DIR)/main.s
	$(AS) $(ASFLAGS) -o $(OUT_DIR)/main.o $(OUT_DIR)/main.s

$(OUT_DIR)/io.s: $(SRC_DIR)/io.c 
	$(XCC) -S $(CFLAGS) $(SRC_DIR)/io.c

$(OUT_DIR)/io.o: $(OUT_DIR)/io.s
	$(AS) $(ASFLAGS) -o $(OUT_DIR)/io.o $(OUT_DIR)/io.s

main.elf: $(OUT_DIR)/main.o $(OUT_DIR)/io.o
	$(LD) $(LDFLAGS) -o $@ $(OUT_DIR)/main.o $(OUT_DIR)/io.o -lgcc

clean:
	-rm -f $(OUT_DIR)/main.elf $(OUT_DIR)/*.s $(OUT_DIR)/*.o $(OUT_DIR)/main.map
