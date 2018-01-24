Team 02 - Zac Davidsen, Trey Lewis, Chris Nurrenberg - 1/24/18

In milestone 3, we added kernel interrupts for loading files,
executing programs, and for terminating.  We also implemented
a simple shell program with two commands, type - finds and 
prints the specified file in the directory, and execute - finds 
the specefied file in the directory, loads it into memory, and 
executes it.

The project can be compiled with "make" or "make all", and 
can be run with "make run".

Known bugs:
    the execute shell command will execute previously 
    executed programs if not provided with a new program name

Additional features:
    created a bash shell script that can print the directory
    from the floppy (./disas.sh -pDir), or disassemble blocks
    on the floppy (./disas.sh [sectors])