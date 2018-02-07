/* Written by (Team 02) Chris Nurrenberg, Zac Davidsen, Trey Lewis 1/31/18*/

#include "./paramPass.h"
#include "./helpers.h"

void printString(char* str);
void readString(char* line);
void readSector(char* buffer, int sector);
void readFile(char *name, char *buffer, int *readCount);
void executeProgram(char *name, int* pid, Params *params);
void terminate();
void writeSector(char *name, int segment);
void deleteFile(char *name);
void writeFile(char *name, char* data, int sectors);
void clearScreen();
void killProcess(int pid);
void blockProcess(int blocking_pid);
int getKeypress();

typedef struct pstruct {
  int active;
  int sp;
  int wait_id;
}pStructs;

int currentProcess;

pStructs ptable[8];


int main() {
  int i;
  char *shell;
  Params defParams;
  makeInterrupt21();

  //printhex(interrupt(0x16, 0x0900, 0, 0, 0));
  
  currentProcess = 0;
  for (i = 0; i < 8; i++) {
    ptable[i].active = 0;
    ptable[i].sp = 0xff00;
    ptable[i].wait_id = -1;
  }

  makeTimerInterrupt();

  shell = "shell";
  defParams.argc = 1;
  defParams.argv = &shell;

  executeProgram("shell", 0, &defParams);

  while (1)
    continue;

  return 0;
}

/* Syscalls */
void handleInterrupt21(int ax, int bx, int cx, int dx) {
  Params defaultParams;
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
    if (dx == 0) {
      defaultParams.argc = 1;
      defaultParams.argv = &bx;
      executeProgram(bx, cx, &defaultParams);
    } else {
      executeProgram(bx, cx, dx);
    }
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
  case 9:
    killProcess(bx);
    break;
  case 10:
    blockProcess(bx);
    break;
  case 11:
    *(int*)bx = getKeypress();
    break;
  default:
    printString(error);
  }
}

/* Scheduler */
void handleTimerInterrupt(int segment, int sp) {
  int i, newSeg, newSp;
  newSp = sp;
  newSeg = div(segment, 0x1000) - 2;
  
  if (newSeg > 7)
    returnFromTimer(segment, sp);

  setKernelDataSegment();

  ptable[newSeg].sp = newSp;
  for (i = newSeg + 1; i < newSeg + 9; i++) {
    if (ptable[mod(i, 8)].active == 1) {
       newSp = ptable[mod(i, 8)].sp;
       newSeg = mod(i,8);
       currentProcess = mod(i,8);
      break;
    }
  }
  
  restoreDataSegment();

  returnFromTimer((newSeg + 2) * 0x1000, newSp);
}

/* Process Management functions */
void executeProgram(char *name, int* pid, Params *params) {
  char buffer[13312];
  char *temp;
  int i, j, sectorsRead, segment, procIndex;

  setKernelDataSegment();

  for (i = 0; i < 8; i++) {
    if (ptable[i].active == 0) {
      break;
    }
  }

  if (i == 8) {
    printString("No free entry in process table.\n\r");
    restoreDataSegment();
    return;
  }
  procIndex = i;
  restoreDataSegment();

  segment = (i + 2) * 0x1000;

  readFile(name, buffer, &sectorsRead);

  if (sectorsRead == -1) {
    return;
  }

  for (i = 0; i < 512*sectorsRead; i++) {
    putInMemory(segment, i, buffer[i]);
  }
  initializeProgram(segment);
  /* Initialize arguments */
  putInMemory(segment, 0xff1a, params->argc);
  putInMemory(segment, 0xff1b, params->argc >> 8);

  sectorsRead = 0xff1e + params->argc * 2;
  putInMemory(segment, 0xff1c, 0x1e);
  putInMemory(segment, 0xff1d, 0xff);
  for (i = 0; i < params->argc; i++) {
    temp = params->argv[i];
    putInMemory(segment, 0xff1e + i*2, sectorsRead);
    putInMemory(segment, 0xff1f + i*2, sectorsRead >> 8);
    for (j = 0; sectorsRead < 0xffff; j++) {
      putInMemory(segment, sectorsRead, temp[j]);
      sectorsRead++;
      if (temp[j] == 0) break;
    }
  }

  if (sectorsRead == 0xfffe) {
    putInMemory(segment, 0xffff, 0);
    setKernelDataSegment();
    printString("Ran out of memory for ARGV!!\n\r");
    restoreDataSegment();
  }
  /* end argument initialization */
  
  setKernelDataSegment();
  ptable[procIndex].sp = 0xff00;
  ptable[procIndex].active = 1;
  restoreDataSegment();
  if (pid != 0) {
    *pid = procIndex;
  }
}

void terminate() {
  int i;
  int count;
  count = 0;

  setKernelDataSegment();

  ptable[currentProcess].active = 0;

  for (i = 0; i < 8; i++) {
    if (ptable[i].wait_id == currentProcess) {
      ptable[i].wait_id = -1;
      ptable[i].active = 1;
    }
    if (ptable[i].active != 0) {
      count++;
    }
  }

  if (count == 0) {
    interrupt(0x15, 0x5301, 0, 0, 0);
    interrupt(0x15, 0x5307, 1, 3, 0);
  }

  restoreDataSegment();

  while (1);

}

void killProcess(int pid) {
  int temp;
  temp = pid;
  setKernelDataSegment();
  if (temp <= 0 || temp > 7) {
    printString("Incorrect process ID.\n\r");
    restoreDataSegment();
    return;
  }

  ptable[temp].active = 0;

  restoreDataSegment();
}

void blockProcess(int blocking_pid) {
  int temp, temp_id;
  temp = blocking_pid;
  setKernelDataSegment();
  temp_id = currentProcess;
  ptable[currentProcess].wait_id = temp;
  ptable[currentProcess].active = 2;
  while (ptable[temp_id].active == 2) {
    continue;
  }
  restoreDataSegment();
}

/* Kernel provided helper functions */
void printString(char *str) {
  char al, ah;
  int ax, i;

  for (i = 0;; i++) {
    al = str[i];
    if (al == 0)
      break;
    ah = 0xE;
    ax = ah * 256 + al;
    interrupt(0x10, ax, 0x07, 0, 0);
  }
}

int getKeypress() {
  return interruptwah(0x16, 0x0000, 0, 0, 0);
}

void readString(char *line) {
  char temp, al, ah;
  int ax, i;
  i = 0;
  while (1) {
    temp = getKeypress();
    //printhex(temp);
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
      interrupt(0x10, ax, 0x07, 0, 0);
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

void readFile(char *file, char *buffer, int *readCount) {
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

  *readCount = -1;
  return;
}

void writeSector(char *name, int segment) {
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

void deleteFile(char *name) {
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
          for (j = j + 1; j < 6; j++) {
            dir[32 * i + j] = 0;
          }
        }
      }
      /* wrote file name, find free sectors, write to dir / map*/
      sectorsWritten = 0;
      for (j = 0; j < 512 && sectorsWritten < sectors; j++) {
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

// int strnCmp(char *str1, char *str2, int length) {
//   int i, ret;

//   for (i = 0; i < length; i++) {
//     ret = str1[i] - str2[i];
//     if (ret != 0)
//       return ret;

//     if (str1[i] == 0)
//       return 0;
//   }
//   return 0;
// }

// int div(int a, int b) {
//   int count;
//   count = 0;
//   while (a >= b) {
//     a = a-b;
//     count++;
//   }
//   return count;
// }

// int mod(int a, int b) {
//   while (a >= b) {
//     a = a-b;
//   }
//   return a;
// }
