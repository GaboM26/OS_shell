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
print to stderr will generate another error so shell exits. If stdout fails,
however, errPrint is called to write to stderr.

ASSUMPTION: Due to the requirement in lab prompt 6, perror is not called
directly but I rather call my function errPrint() in order to output the
errors in the way the prompt requires us to. 

-------------------------------------------------------
Part 2|
-------
