Team 02 - Zac Davidsen, Trey Lewis, Chris Nurrenberg - 1/31/18

During this milestone, we added kernel interrupts to write 
data to sectors on the disk, and to delete files.  We added
delete and copy commands to the shell, and a dir command
which prints files in the directory and their size in sectors.
We also added the create command to the shell, which reads
from the shell until a blank newline is entered, then writes
the input to a new file (without newlines).

The project can be compiled by running "make" or "make all",
and can be run with "make run", which will also recompile.

Known bugs:
    there are no known bugs at this time

Additional features:
    dir prints the size of the file (in sectors)
    added shell clear command
    added shell bgcol command, which cycles background colors