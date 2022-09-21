CC = gcc
INCLUDES = -std=c99
CFLAGS = -m16 -ffreestanding -fno-PIE -nostartfiles -nostdlib 
LDFLAGS = -g
LDLIBS = 

floppy.flp: main.elf
	objcopy -O binary main.elf floppy.flp

main.elf: entry.o main.o linker.ld
	ld -m elf_i386 -o main.elf -T linker.ld entry.o main.o 

main.o: main.c
	$(CC) -c $(CFLAGS) -o main.o $(INCLUDES) main.c

entry.o: entry.S
	as --32 -o entry.o entry.S

.PHONY: clean
clean:
	rm -f *.o a.out core main *.txt main.elf floppy.flp

.PHONY: all
all: clean floppy.flp

