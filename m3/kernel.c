/* Written by (Team 02) Chris Nurrenberg, Zac Davidsen, Trey Lewis 1/20/18*/

void printString(char* str);
void readString(char* line);
void readSector(char* buffer, int sector);
void readFile(char *name, char *buffer);
void executeProgram(char *name, int segment);
void terminate();

int main() {
  char buffer[13312];
  makeInterrupt21();

  interrupt(0x21, 4, "shell", 0x2000, 0);

  /*interrupt(0x21, 3, "messag", buffer, 0);
  interrupt(0x21, 0, buffer, 0, 0);

  interrupt(0x21, 4, "tstpr2", 0x2000, 0);*/

  while (1)
    continue;

  return 0;
}

void handleInterrupt21(int ax, int bx, int cx, int dx) {
  char error[22];
  error[0] = 'I';
  error[1] = 'n';
  error[2] = 'c';
  error[3] = 'o';
  error[4] = 'r';
  error[5] = 'r';
  error[6] = 'e';
  error[7] = 'c';
  error[8] = 't';
  error[9] = ' ';
  error[10] = 'a';
  error[11] = 'x';
  error[12] = ' ';
  error[13] = 'v';
  error[14] = 'a';
  error[15] = 'l';
  error[16] = 'u';
  error[17] = 'e';
  error[18] = '.';
  error[19] = '\n';
  error[20] = '\r';
  error[21] = '\0';

  switch (ax) {
  case 0:
    printString(bx);
    break;
  case 1:
    readString(bx);
    break;
  case 2:
    readSector(bx, cx);
    break;
  case 3:
    readFile(bx, cx);
    break;
  case 4:
    executeProgram(bx, cx);
    break;
  case 5:
    terminate();
    break;
  default:
    printString(error);
  }
}

int div(int a, int b) {
  int count;
  count = 0;
  while (a >= b) {
    a = a-b;
    count++;
  }
  return count;
}

int mod(int a, int b) {
  while (a >= b) {
    a = a-b;
  }
  return a;
}

void printString(char *str) {
  char al, ah;
  int ax, i;

  for (i = 0;; i++) {
    al = str[i];
    if (al == 0)
      break;
    ah = 0xE;
    ax = ah * 256 + al;
    interrupt(0x10, ax, 0, 0, 0);
  }
}

void readString(char *line) {
  char temp, al, ah;
  int ax, i;
  i = 0;
  while (1) {
  ax = 0;
  temp = interrupt(0x16, ax, 0, 0, 0);
  if (temp == 0xd) {
    line[i] = 0xa;
    line[i+1] = 0xd;
    line[i+2] = 0x0;
    ax = 0xE * 256 + 0xa;
    interrupt(0x10, ax, 0, 0, 0);
    ax = 0xE * 256 + 0xd;
    interrupt(0x10, ax, 0, 0, 0);
    return;
  } else if (temp == 0x8) {
    if ( i > 0 )
      i--;
    ax = 0xE * 256 + 0x8;
    interrupt(0x10, ax, 0, 0, 0);
    ax = 0xE * 256 + ' ';
    interrupt(0x10, ax, 0, 0, 0);
    ax = 0xE * 256 + 0x8;
    interrupt(0x10, ax, 0, 0, 0);
  } else {
    line[i] = temp;
    i++;
    ax = 0xE * 256 + temp;
    interrupt(0x10, ax, 0, 0, 0);
  }
}
}

void readSector(char *buffer, int sector) {
  char ah, al, ch, cl, dh, dl;
  int ax, bx, cx, dx;

  ah = 2;
  al = 1;
  bx = buffer;
  ch = div(sector, 36);
  cl = mod(sector, 18) + 1;
  dh = mod(div(sector, 18), 2);
  dl = 0;

  ax = ah*256 + al;
  cx = ch*256 + cl;
  dx = dh*256 + dl;

  interrupt(0x13, ax, bx, cx, dx);
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

void readFile(char *file, char *buffer) {
  char dir[512];
  int entry, field, comp;
  entry = 0;
  field = 0;

  readSector(dir, 2);

  for (entry = 0; entry < 16; entry++) {
    comp = strnCmp(file, dir + entry * 32, 6);
    if (comp == 0) {
      for (field = 6; field < 32; field++) {
        comp = dir[entry * 32 + field];
        if (comp == 0)
          return;
        readSector(buffer + 512 * (field - 6), comp);
      }
    }
  }
}

void executeProgram(char *name, int segment) {
  char buffer[13312];
  int i;

  readFile(name, buffer);

  for (i = 0; i < 13312; i++) {
    putInMemory(segment, i, buffer[i]);
  }

  launchProgram(segment);
}

void terminate() {
  char str[6];
  str[0] = 's';
  str[1] = 'h';
  str[2] = 'e';
  str[3] = 'l';
  str[4] = 'l';
  str[5] = '\0';

  interrupt(0x21, 4, str, 0x2000, 0);
}