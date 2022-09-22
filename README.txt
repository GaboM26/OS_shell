################Programming Problem 1##################
Name: David Gabriel Millares Bellido
UNI: DGM2148
-------------------------------------------------------
Part 1|
-------
My w4118_sh works just like what is expected with the assignment.
I implemented cd and exit, while all other commands (I treat all things that
don't have a path attached to them as commands) print a "Unknown command"
error. cd takes one command line argument while exit takes no command line
arguments. Any extra command line arguments given into exit will still just
exit. In the case of cd, the first command line argument will be treated as
the directory to change to. 

ASSUMPTION: If write to stderr fails, program will simply exit as next call
will simply generate another error in stderr. I assume that another call to
print to stderr will generate another error so shell exits. If a write to stdout 
fails, however, errPrint is called to write to stderr.

ASSUMPTION: Due to the requirement in lab prompt 6, perror is not called
directly but I rather call my function errPrint() in order to output the
errors in the way the prompt requires us to. 

NO ERRORS OR WARNINGS IN CHECKPATCH/VALGRIND
...............................................................................
Test Run|
---------
NOTE THAT ONLY STDOUT AND STDERR WERE APPENDED, THE COMMANDS I RAN WERE:

/bin/pwd
cd ..
/bin/pwd
/bin/ls
cd part1
/bin/cat Makefile
cd /home/gabomba
./a.out
cd w4118
../a.out
/bin/pwd
cd ..
./w4118
/
ls
Hello
exit

==4401== Memcheck, a memory error detector
==4401== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==4401== Using Valgrind-3.16.1 and LibVEX; rerun with -h for copyright info
==4401== Command: ./w4118_sh
==4401== 
/home/gabomba/w4118/lab1/f22-hmwk1-GaboM26/part1
/home/gabomba/w4118/lab1/f22-hmwk1-GaboM26
helloWorldOS.png
part1
part2
part3
README.txt
references.txt
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
Hello World 
Hello World 
/home/gabomba/w4118
error: Permission denied
$$$$$$$$$$$$$==4409== 
==4409== HEAP SUMMARY:
==4409==     in use at exit: 0 bytes in 0 blocks
==4409==   total heap usage: 3 allocs, 3 frees, 5,220 bytes allocated
==4409== 
==4409== All heap blocks were freed -- no leaks are possible
==4409== 
==4409== For lists of detected and suppressed errors, rerun with: -s
==4409== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
error: Permission denied
$$$$$$$$$$$$$$==4410== 
==4410== HEAP SUMMARY:
==4410==     in use at exit: 0 bytes in 0 blocks
==4410==   total heap usage: 3 allocs, 3 frees, 5,220 bytes allocated
==4410== 
==4410== All heap blocks were freed -- no leaks are possible
==4410== 
==4410== For lists of detected and suppressed errors, rerun with: -s
==4410== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
error: unknown command
error: unknown command
$$$$$$$$$$$$$$$$$==4401== 
==4401== HEAP SUMMARY:
==4401==     in use at exit: 0 bytes in 0 blocks
==4401==   total heap usage: 3 allocs, 3 frees, 5,220 bytes allocated
==4401== 
==4401== All heap blocks were freed -- no leaks are possible
==4401== 
==4401== For lists of detected and suppressed errors, rerun with: -s
==4401== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
...............................................................................
Test Run using executable|
--------------------------
2 of 2 tests passed
5 of 5 tests passed
9 of 9 tests passed

I found the test was very useful for trying out errors like just inputing
a cd without any path, but it became difficult to try something like '/bin/ls
-lha'. Those kinds of tests I tried manually. 

NOTE: I have moved the homework_tests.py file and commited it along with the
rest of the documentation so you have access to tests I ran.
-------------------------------------------------------------------------------
Part 2|
-------
