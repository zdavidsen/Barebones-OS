# Barebones-OS
Bare metal OS built for my Operating Systems course

A simple operating system built from scratch (kinda), designed to run from a floppy disk on an i386 processor.  
Main contributors/project group members were Chris Nurrenberg(nurrencd), Trey Lewis(lewistd), and myself.  
There are also various commits from the course instructor, Delvin Defoe, and TAs who graded the project.

The most up to date version is currently in m5, but I may add another folder and continue development there eventually.

The project was structured to have five distinct milestones, hence the folders labeled m1-m5.  
For each milestone, we built on the previous code we had written, and were usually given some additional helper files.
We did not have to write any assembly ourselves, as it was all provided, though some modifications were made.

The project is separated into two main parts, the kernel and shell, in their own respective files.
The kernel is basically just an IPL for the shell, and provides syscalls to running programs. 
It also implements multiprocessing with a round robin scheduler, and a simple blocking mechanism for parent processes.

The shell is fairly simple overall, but offers the basic utilities.  It lacks a help command (might address that at some point),
but offers tab auto completion (which lists all commands if nothing is typed) and command history over the current session.
As it is, all commands are built into the shell executable, though it can execute other programs through the execute command.  

### Notable Shell features
* Command line arguments!  Admittedly a really hacky solution, and probably breaks on different compilers.  Need to look into the correct way of doing this.
* Tab auto-complete.  Pretty much exactly what you're used to, just only applies to built in command names
* Command history.  Remembers the last 20 commands from the current session
* Display paging (beta?).  pg1 and pg2 commands change display pages, and pages maintain their content when inactive (except graphics changes or clear command?).  Printing always happens on the active page
* Draw.  Changes to graphics mode, spacebar to draw a "pixel", + or - to change pixel size, arrow keys to change pixel position, 0-9 a-f to change color
* Most other commands are self explanatory, except type, which is the equivalent of linux cat.
