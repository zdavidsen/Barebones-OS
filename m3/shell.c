/* Written by (Team 02) Chris Nurrenberg, Zac Davidsen, Trey Lewis 1/20/18*/

#define PRINTSTRING 0x21, 0
#define READSTRING 0x21, 1
#define READSECTOR 0x21, 2
#define READFILE 0x21, 3
#define EXECUTEPROGRAM 0x21, 4
#define TERMINATE 0x21, 5

int strnCmp(char *str1, char *str2, int length);

int main() {
  char cmdBuf[80];
  int i;
  char buffer[13312];

  while (1) {
    for (i = 0; i < 80; i++) {
      cmdBuf[i] = 0;
    }

    interrupt(PRINTSTRING, "SHELL: ", 0, 0);
    interrupt(READSTRING, cmdBuf, 0, 0);

    if (strnCmp(cmdBuf, "type", 4) == 0) {
      interrupt(READFILE, cmdBuf + 5, buffer);
      interrupt(PRINTSTRING, buffer, 0, 0);
      interrupt(PRINTSTRING, "\n\r", 0, 0);
    } else if (strnCmp(cmdBuf, "execute", 7) == 0) {
      interrupt(EXECUTEPROGRAM, cmdBuf + 8, 0x3000);
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
