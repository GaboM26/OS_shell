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
My w4118_sh2 works just as what is expected. I started with the sample of my
part1 shell and removed all I/O functions one by one. The first I replaced
was printf() with myPrint(). The next was fprintf() with errPrint(),
a function I already had, but was now re-purposed to print out errors just
like what the prompt required us to. errPrint() only prints out to stderr,
while myPrint() only prints to stdout. The next function I implemented was
myMalloc(), which calls mmap and is inspired by the bootlin code. I decided
to not implement a function called myRealloc() as a replacement for the
realloc() in getLine(), but instead just wrote the same functionality as
realloc() within myGetLine(). myGetLine() calls myMalloc() and keeps
allocating more space as needed. In order to test if this works, one can
reduce the sizes of the buffer in the main function to 10 and the ones in
the myGetLine() function to 3 each, and then run a 'long' command like
$/bin/ls -lha /home/gabomba[or any directory in home]. Finally, instead of
calling free, my file just directly runs munmap() whenever needed, including
in myExit() function. 

ASSUMPTION: I copied the shell from part 1 because the prompt in part 2 said
"modify your simple shell". I understood that the implication is that we
could copy the code I wrote in part 1 and modify it to call system calls
directly. 

SMALL ERROR: I did not find out how to deal with input in which the user
presses Ctrl+D (EOF). The problem is that when this happens, I discovered
that my "realloc()" part in myGetLine() would allocate 1000 more bytes for
the buffer in the heap. This goes unnoticed for the first few times this is
done, but could lead to problems as [Command] Ctrl+D presses go to infinity. 
Apart from this very specific case, memory allocation doesn't grow 
indefinitely at all. Furthermore, my testing showed me that all the
allocated memory ended up being deallocated properly, so this was not an
issue at all.

NO ERRORS OR WARNINGS IN CHECKPATCH/VALGRIND
...............................................................................
Test run|
---------
COMMANDS I RAN:
cd ..
/bin/pwd
/
cd
/bin
/bin/cat references.txt
cd /
/bin/pwd
/home/gabomba/a.out
exit

==5823== Memcheck, a memory error detector
==5823== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==5823== Using Valgrind-3.16.1 and LibVEX; rerun with -h for copyright info
==5823== Command: ./w4118_sh2
==5823== 
$$/home/gabomba/w4118/lab1/f22-hmwk1-GaboM26
$error: Permission denied
==5829== 
==5829== HEAP SUMMARY:
==5829==     in use at exit: 0 bytes in 0 blocks
==5829==   total heap usage: 0 allocs, 0 frees, 0 bytes allocated
==5829== 
==5829== All heap blocks were freed -- no leaks are possible
==5829== 
==5829== For lists of detected and suppressed errors, rerun with: -s
==5829== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
$error: unknown command
$error: Permission denied
==5831== 
==5831== HEAP SUMMARY:
==5831==     in use at exit: 0 bytes in 0 blocks
==5831==   total heap usage: 0 allocs, 0 frees, 0 bytes allocated
==5831== 
==5831== All heap blocks were freed -- no leaks are possible
==5831== 
==5831== For lists of detected and suppressed errors, rerun with: -s
==5831== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
$Reference List for W4118 Homework

1.
https://stackoverflow.com/questions/11824685/what-does-the-second-parameter-of-waitpid-mean,
used in order to remember a bit how the waitpid function worked and what
status was for

2. https://cs3157.github.io/www/2022-1/resources/guides/git-tutorial.html
Used in order for a refresher on how to use git

3. https://linuxhint.com/check-malloc-error-c/
I was unsure on how to handle a malloc error so I used this for reference

4. http://www.cs.columbia.edu/~jae/3157/code/shell.c
Im not sure if this must be cited because it was shown in class, but I put
it here to be on the safe side. Looked at shell.c for the fork/exec lines of
code

5. https://elixir.bootlin.com/linux/latest/source/tools/include/nolibc/stdlib.h#L112
Given with OS lab spec, used for help to implement malloc and realloc. Note
that I don't directly implement a function named realloc, I just simply
wrote code that does the same thing as realloc and placed it at the bottom of
myGetLine() function

6. https://commons.wikimedia.org/wiki/File:ASCII-Table-wide.svg
Used to see the actual ASCII values of the corresponding chars to fill in the main.c file. 

David Gabriel Millares Bellido
DGM2148

$$/
$Hello World 
$==5823== 
==5823== HEAP SUMMARY:
==5823==     in use at exit: 0 bytes in 0 blocks
==5823==   total heap usage: 0 allocs, 0 frees, 0 bytes allocated
==5823== 
==5823== All heap blocks were freed -- no leaks are possible
==5823== 
==5823== For lists of detected and suppressed errors, rerun with: -s
==5823== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
...............................................................................
strace run|
-----------
I added an strace run with the same commands as before:

execve("./w4118_sh2", ["./w4118_sh2"], 0x7ffe3fe57050 /* 30 vars */) = 0
brk(NULL)                               = 0x55c349ddd000
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
fstat(3, {st_mode=S_IFREG|0644, st_size=73680, ...}) = 0
mmap(NULL, 73680, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7fd604683000
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0@n\2\0\0\0\0\0"..., 832) = 832
fstat(3, {st_mode=S_IFREG|0755, st_size=1839792, ...}) = 0
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7fd604681000
mmap(NULL, 1852680, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7fd6044bc000
mprotect(0x7fd6044e1000, 1662976, PROT_NONE) = 0
mmap(0x7fd6044e1000, 1355776, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x25000) = 0x7fd6044e1000
mmap(0x7fd60462c000, 303104, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x170000) = 0x7fd60462c000
mmap(0x7fd604677000, 24576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1ba000) = 0x7fd604677000
mmap(0x7fd60467d000, 13576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7fd60467d000
close(3)                                = 0
arch_prctl(ARCH_SET_FS, 0x7fd604682540) = 0
mprotect(0x7fd604677000, 12288, PROT_READ) = 0
mprotect(0x55c3497f3000, 4096, PROT_READ) = 0
mprotect(0x7fd6046bf000, 4096, PROT_READ) = 0
munmap(0x7fd604683000, 73680)           = 0
mmap(NULL, 100, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7fd6046be000
write(1, "$", 1)                        = 1
read(0, "cd ..\n", 99)                  = 6
chdir("..")                             = 0
write(1, "$", 1)                        = 1
read(0, "/bin/pwd\n", 99)               = 9
clone(child_stack=NULL, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLD, child_tidptr=0x7fd604682810) = 5854
wait4(5854, [{WIFEXITED(s) && WEXITSTATUS(s) == 0}], 0, NULL) = 5854
--- SIGCHLD {si_signo=SIGCHLD, si_code=CLD_EXITED, si_pid=5854, si_uid=1000, si_status=0, si_utime=0, si_stime=0} ---
write(1, "$", 1)                        = 1
read(0, "/\n", 99)                      = 2
clone(child_stack=NULL, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLD, child_tidptr=0x7fd604682810) = 5855
wait4(5855, [{WIFEXITED(s) && WEXITSTATUS(s) == 0}], 0, NULL) = 5855
--- SIGCHLD {si_signo=SIGCHLD, si_code=CLD_EXITED, si_pid=5855, si_uid=1000, si_status=0, si_utime=0, si_stime=0} ---
write(1, "$", 1)                        = 1
read(0, "cd\n", 99)                     = 3
write(2, "error: unknown command\n", 23) = 23
write(1, "$", 1)                        = 1
read(0, "/bin\n", 99)                   = 5
clone(child_stack=NULL, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLD, child_tidptr=0x7fd604682810) = 5856
wait4(5856, [{WIFEXITED(s) && WEXITSTATUS(s) == 0}], 0, NULL) = 5856
--- SIGCHLD {si_signo=SIGCHLD, si_code=CLD_EXITED, si_pid=5856, si_uid=1000, si_status=0, si_utime=0, si_stime=0} ---
write(1, "$", 1)                        = 1
read(0, "/bin/cat references.txt\n", 99) = 24
clone(child_stack=NULL, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLD, child_tidptr=0x7fd604682810) = 5857
wait4(5857, [{WIFEXITED(s) && WEXITSTATUS(s) == 0}], 0, NULL) = 5857
--- SIGCHLD {si_signo=SIGCHLD, si_code=CLD_EXITED, si_pid=5857, si_uid=1000, si_status=0, si_utime=0, si_stime=0} ---
write(1, "$", 1)                        = 1
read(0, "cd /\n", 99)                   = 5
chdir("/")                              = 0
write(1, "$", 1)                        = 1
read(0, "/bin/pwd\n", 99)               = 9
clone(child_stack=NULL, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLD, child_tidptr=0x7fd604682810) = 5858
wait4(5858, [{WIFEXITED(s) && WEXITSTATUS(s) == 0}], 0, NULL) = 5858
--- SIGCHLD {si_signo=SIGCHLD, si_code=CLD_EXITED, si_pid=5858, si_uid=1000, si_status=0, si_utime=0, si_stime=0} ---
write(1, "$", 1)                        = 1
read(0, "/home/gabomba/a.out\n", 99)    = 20
clone(child_stack=NULL, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLD, child_tidptr=0x7fd604682810) = 5859
wait4(5859, [{WIFEXITED(s) && WEXITSTATUS(s) == 0}], 0, NULL) = 5859
--- SIGCHLD {si_signo=SIGCHLD, si_code=CLD_EXITED, si_pid=5859, si_uid=1000, si_status=0, si_utime=0, si_stime=0} ---
write(1, "$", 1)                        = 1
read(0, "exit\n", 99)                   = 5
munmap(0x7fd6046be000, 100)             = 0
exit_group(0)                           = ?
+++ exited with 0 +++
...............................................................................
test run using executable|
--------------------------
2 of 2 tests passed
5 of 5 tests passed
9 of 9 tests passed

Tests with this shell ran smoothly once mmap and munmap were figured out.
Another mistake I faced early on was not null terminating input I received
from read(). In the end, part 2 worked just as required.
-------------------------------------------------------------------------------
Part 3|
-------

