/* Written by (Team 02) Chris Nurrenberg, Zac Davidsen, Trey Lewis 1/17/18*/
#include "./functions.h"

int main() {
char line[512];
char *hello;

hello = "Hello World";

printString(hello);

printString("Enter a new line: \0");

readString(line);

printString(line);

readSector(line, 30);
printString(line);

while(1)
  continue;

return 0;
}
