/* Written by (Team 02) Chris Nurrenberg, Zac Davidsen, Trey Lewis 1/20/18*/

#define PRINTSTRING 0x21, 0
#define READSTRING 0x21, 1
#define READSECTOR 0x21, 2
#define READFILE 0x21, 3
#define EXECUTEPROGRAM 0x21, 4
#define TERMINATE 0x21, 5
#define WRITESECTOR 0x21, 6
#define DELETEFILE 0x21, 7
#define WRITEFILE 0x21, 8

int strnCmp(char *str1, char *str2, int length);
void parseArguments(char *args, int *argc, char **argv);

int main() {
  char cmdBuf[80];
  int i;
  char buffer[13312];
  int argCount;
  char *argArray[5];
  int readCount;

  while (1) {
    for (i = 0; i < 80; i++) {
      cmdBuf[i] = 0;
    }
    for (i = 0; i < 13312; i++) {
      buffer[i] = 0;
    }

    interrupt(PRINTSTRING, "SHELL: ", 0, 0);
    interrupt(READSTRING, cmdBuf, 0, 0);

    
    parseArguments(cmdBuf, &argCount, argArray);

    if (strnCmp(argArray[0], "type", 4) == 0 && argCount >= 2) {
      interrupt(READFILE, argArray[1], buffer, &readCount);
      interrupt(PRINTSTRING, buffer, 0, 0);
      interrupt(PRINTSTRING, "\n\r", 0, 0);
    } else if (strnCmp(argArray[0], "execute", 7) == 0 && argCount >= 2) {
      interrupt(EXECUTEPROGRAM, argArray[1], 0x3000);
    } else if (strnCmp(argArray[0], "delete", 6) == 0) {
      interrupt(DELETEFILE, argArray[1], 0, 0);
    } else if (strnCmp(argArray[0], "copy", 4) == 0 && argCount == 3) {
      interrupt(READFILE, argArray[1], buffer, &readCount);
      if (readCount > 0) {
        interrupt(WRITEFILE, argArray[2], buffer, readCount);
      } else {
        interrupt(PRINTSTRING, "FILE NOT FOUND\n\r", 0, 0);
      }
    } else {
      interrupt(PRINTSTRING, "Invalid Command\n\r", 0, 0);
    }
  }
}

int strnCmp(char *str1, char *str2, int length) {
  int i, ret;

  for (i = 0; i < length; i++) {
    ret = str1[i] - str2[i];
    if (ret != 0)
      return ret;

    if (str1[i] == 0)
      return 0;
  }
  return 0;
}

void parseArguments(char *args, int *argc, char **argv){
  int i, inArg;
  *argc = 0;
  inArg = 0;


  i = 0;
  while(args[i] != '\0') {
    if (args[i] != ' '){
      if (!inArg){
        (argv)[*argc] = args + i;
        (*argc)++;
      }
      inArg = 1;
    }
    else if (args[i] == ' '){
      inArg = 0;
      args[i] = 0x0;
    }
    i++;
  }
}

int div(int a, int b){
  int i;
  i = 0;
  while (a >= b){
    a = a - b;
    i++;
  }
  return i;
}
