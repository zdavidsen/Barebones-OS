
In this milestone, we implemented multiprocessing with a process table and
round robin execution, and the ability to execute processes in either the 
foreground or background.  In orderto implement foreground execution, we 
implemented a blockProcess function that takes a pid as a parameter and halts 
the calling process until the blocking process is complete.  We also implemented 
the kill command.

to compile the os, "make" or "make all"
to run the os, "make run". this will also incur "make all"

Known bugs:
    Executing two processes in the background that both take keyboard input is a 
        very quick way to crash the os.
    Backspacing does not handle multiple lines of input (works fine functionaly,
        just doesn't change the display)
    
Extra features:
    clear screen command
    display paging (pg1, pg2)
    changing background color, which accepts a single hex digit for color (bgcol 1)
    dir shows size in sectors of files
    draw command, changes to graphics mode and uses 0-9,a-f to set color, arrow
        keys or ijkl to navigate the screen, space bar to print a pixel, and +/-
        to change the size of printed pixels
    tab completion of command names in shell (no arguments though)
    command line parameter passing to executed programs (test with "execute printa
        [up to 8 args]") only 8 args because that's the size of the array.
    exit command, exits shell. Computer turns off when all processes terminate