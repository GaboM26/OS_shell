CC = gcc
INCLUDES =
CFLAGS = -g -Wall $(INCLUDES)
LDFLAGS = -g
LDLIBS = 

w4118_sh: w4118_sh.o

w4118_sh.o: shell.c
	$(CC) -c shell.c $(CFLAGS) -o w4118_sh.o

.PHONY: clean
clean:
	rm -f *.o a.out core w4118_sh

.PHONY: all
all: clean w4118_sh

.PHONY: do
do: all
	./w4118_sh

.PHONY: val
val: all
	valgrind --leak-check=yes ./w4118_sh
