/* Written by (Team 02) Chris Nurrenberg, Zac Davidsen, Trey Lewis 1/20/18*/

void printString(char* str);
void readString(char* line);
void readSector(char* buffer, int sector);
void readFile(char *name, char *buffer, int *readCount);
void executeProgram(char *name, int segment);
void terminate();
void writeSector(char *name, int segment);
void deleteFile(char *name);
void writeFile(char *name, char* data, int sectors);

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
    readFile(bx, cx, dx);
    break;
  case 4:
    executeProgram(bx, cx);
    break;
  case 5:
    terminate();
    break;
  case 6:
    writeSector(bx, cx);
    break;
  case 7:
    deleteFile(bx);
    break;
  case 8:
    writeFile(bx, cx, dx);
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
    line[i] = 0;
    /* line[i] = 0xa;
    line[i+1] = 0xd;
    line[i+2] = 0x0; */
    ax = 0xE * 256 + 0xa;
    interrupt(0x10, ax, 0, 0, 0);
    ax = 0xE * 256 + 0xd;
    interrupt(0x10, ax, 0, 0, 0);
    return;
  } else if (temp == 0x8) {
    if ( i > 0 ) {
      i--;
      ax = 0xE * 256 + 0x8;
      interrupt(0x10, ax, 0, 0, 0);
      ax = 0xE * 256 + ' ';
      interrupt(0x10, ax, 0, 0, 0);
      ax = 0xE * 256 + 0x8;
      interrupt(0x10, ax, 0, 0, 0);
    }
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

void readFile(char *file, char *buffer, int *readCount) {
  
  char error[18];
  char dir[512];
  int entry, field, comp, trash;
  entry = 0;
  field = 0;

  if (readCount == 0) {
    readCount = &trash;
  }

  *readCount = 0;

  readSector(dir, 2);

  for (entry = 0; entry < 16; entry++) {
    comp = strnCmp(file, dir + entry * 32, 6);
    if (comp == 0) {
      for (field = 6; field < 32; field++) {
        comp = dir[entry * 32 + field];
        if (comp == 0)
          return;
        readSector(buffer + 512 * (field - 6), comp);
        (*readCount)++;
        if (field == 31) {
          return;
        }
      }
    }
  }

  error[0] = 'F';
  error[1] = 'i';
  error[2] = 'l';
  error[3] = 'e';
  error[4] = ' ';
  error[5] = 'n';
  error[6] = 'o';
  error[7] = 't';
  error[8] = ' ';
  error[9] = 'f';
  error[10] = 'o';
  error[11] = 'u';
  error[12] = 'n';
  error[13] = 'd';
  error[14] = '.';
  error[15] = '\n';
  error[16] = '\r';
  error[17] = '\0';
  printString(error);
  *readCount = -1;
  return ;
}

void executeProgram(char *name, int segment) {
  char buffer[13312];
  int i, sectorsRead;

  readFile(name, buffer, &sectorsRead);

  if (sectorsRead == -1){
    return;
  }

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

void writeSector(char *name, int segment){
  char ah, al, ch, cl, dh, dl;
  int ax, bx, cx, dx;

  ah = 3;
  al = 1;
  bx = name;
  ch = div(segment, 36);
  cl = mod(segment, 18) + 1;
  dh = mod(div(segment, 18), 2);
  dl = 0;

  ax = ah*256 + al;
  cx = ch*256 + cl;
  dx = dh*256 + dl;

  interrupt(0x13, ax, bx, cx, dx);
}

void deleteFile(char *name){
  char error[18];
  char dir[512];
  char map[512];
  int entry, field, comp;
  entry = 0;
  field = 0;

  readSector(dir, 2);
  readSector(map, 1);

  for (entry = 0; entry < 16; entry++) {
    comp = strnCmp(name, dir + entry * 32, 6);
    if (comp == 0) {
      for (field = 6; field < 32; field++) {
        comp = dir[entry * 32 + field];
        if (comp == 0)
          break;
        map[comp] = 0x0;
      }
      dir[entry * 32] = '\0';
      break;
    }
    
  }
  writeSector(map, 1);
  writeSector(dir, 2);
}

void writeFile(char *name, char *data, int sectors) {
  char map[512], dir[512];
  int i, j, sectorsWritten;

  readSector(dir, 2);
  readSector(map, 1);
  /* check amount of free sectors, return if insufficient*/
  j = 0;
  for (i = 0; i < 512; i++) {
    if (map[i] == 0) {
      j++;
    }
  }
  if (j < sectors) {
    /* include error message plz*/
    return;
  }

  for (i = 0; i < 16; i++) {
    if (dir[32 * i] == 0) {
      /* found free sector, write name to directory */
      for (j = 0; j < 6; j++) {
        dir[32 * i + j] = name[j];
        if (name[j] == 0) {
          for(j = j + 1; j < 6; j++) {
            dir[32 * i + j] = 0;
          }
        }
      }
      /* wrote file name, find free sectors, write to dir / map*/
      sectorsWritten = 0;
      for (j = 0; j < 512 && sectorsWritten < sectors; j++){
        if (map[j] == 0) {
          map[j] = 0xFF;
          dir[32 * i + 6 + sectorsWritten] = j;
          writeSector(data + sectorsWritten * 512, j);
          sectorsWritten++;
        }
      }
      for (j = sectorsWritten + 6; j < 32; j++) {
        dir[32 * i + j] = 0;
      }
      /* */
      writeSector(map, 1);
      writeSector(dir, 2);
      return;
    }
  }
}
