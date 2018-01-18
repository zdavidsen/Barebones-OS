/* Written by (Team 02) Chris Nurrenberg, Zac Davidsen, Trey Lewis 1/17/18*/

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

void handleInterrupt21(int ax, int bx, int cx, int dx) {
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
  default:
    printString("Incorrect ax value.\n\r");
  }
}
