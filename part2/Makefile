CC = gcc
INCLUDES =
CFLAGS = -g -Wall $(INCLUDES)
LDFLAGS = -g
LDLIBS = 

w4118_sh2: w4118_sh2.o

w4118_sh2.o: shadow_shell2.c
	$(CC) -c shadow_shell2.c $(CFLAGS) -o w4118_sh2.o

.PHONY: clean
clean:
	rm -f *.o a.out core w4118_sh2 *.txt

.PHONY: all
all: clean w4118_sh2

.PHONY: do
do: all
	./w4118_sh2

.PHONY: val
val: all
	valgrind --leak-check=yes ./w4118_sh2

trace2.txt: w4118_sh2
	strace -o trace2.txt ./w4118_sh2 
