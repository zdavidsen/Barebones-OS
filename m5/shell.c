/* Written by (Team 02) Chris Nurrenberg, Zac Davidsen, Trey Lewis 1/31/18*/

#include "./syscall.h"
#include "./keycode.h"
#include "./paramPass.h"
#include "./helpers.h"

void parseArguments(char *args, int *argc, char **argv);
void listDirectory();
void creatFile(char *name);
void clearScreen();
void drawStuff();
void processInput();
void printhex(int a);
void reorganizeHistory(char **buffer);
void refreshLine();
void refreshBuffer(char *buffer, int size);

int main(int argc, char *argv[]) {
  char cmdBuf[80];
  int i;
  char buffer[13312];
  int argCount;
  char *argArray[10];
  int readCount;
  int pid;
  Params params;
  /* command history stuff*/
  char history[20][80];
  char *commandStrings[13];
  int currentCommand, maxCommands, tempPress, cmdIndex, acceptingInput;
  int tabIndex, tabCount;
  char tabBuffer[80];

  // for (i = 0; i < 20; i++) {
  //   history[i][0] = 0;
  // }
  maxCommands = 0;
  cmdIndex = 0;
  /* set command buffer */
  commandStrings[0] = "type";
  commandStrings[1] = "execute";
  commandStrings[2] = "delete";
  commandStrings[3] = "copy";
  commandStrings[4] = "dir";
  commandStrings[5] = "create";
  commandStrings[6] = "clear";
  commandStrings[7] = "bgcol";
  commandStrings[8] = "kill";
  commandStrings[9] = "execforeground";
  commandStrings[10] = "draw";
  commandStrings[11] = "pg1";
  commandStrings[12] = "pg2";

  enableInterrupts();
  while (1) {
    for (i = 0; i < 80; i++) {
      cmdBuf[i] = 0;
    }
    for (i = 0; i < 13312; i++) {
      buffer[i] = 0;
    }
    /* begin processing input*/ 
    refreshBuffer(tabBuffer, 80);
    cmdIndex = 0;
    currentCommand = 0;
    refreshLine();
    acceptingInput = 1;
    while (acceptingInput) {
      getKeypress(&tempPress);
      switch (tempPress) {
        case KEY_ARROW_UP:
          if (currentCommand < 19){
            if (currentCommand == 0){
              strnCpy(cmdBuf, history[0], strLen(cmdBuf));
            }
            refreshLine();            
            currentCommand++;
            printString(history[currentCommand]);
          }
          break;
        case KEY_ARROW_DOWN:
          if (currentCommand > 0){
            refreshLine();            
            currentCommand--;
            printString(history[currentCommand]);
          }
          break;
        case KEY_ARROW_RIGHT:
        case KEY_ARROW_LEFT:
          break;
        case KEY_ENTER:
          acceptingInput = 0;
          cmdBuf[cmdIndex++] = 0x0;
          printString("\n\r");
          break;
        case KEY_BACKSPACE:
          if (cmdIndex > 0){
            cmdIndex--;
            printString("\b \b");
          }
          cmdBuf[cmdIndex] = '\0';
          break;
        case KEY_TAB:
          tabIndex = 0;
          tabCount = 0;
          for (i = 0; i < 13; i++){
            if (strnCmp(cmdBuf, commandStrings[i], strLen(cmdBuf))==0){
              tabCount++;
              tabIndex += strnCpy(commandStrings[i], tabBuffer + tabIndex,
                strLen(commandStrings[i]));
              tabBuffer[tabIndex++] = ' ';
            }
          }
          if (tabCount == 1){
            tabCount = tabIndex - strLen(cmdBuf);
            strnCpy(tabBuffer + strLen(cmdBuf), cmdBuf + cmdIndex, strLen(tabBuffer));
            printString(cmdBuf + cmdIndex);
            cmdIndex += tabCount;
          } else if (tabCount > 1) {
            printString("\n\r");
            printString(tabBuffer);
            printString("\n\r");
            refreshBuffer(tabBuffer, 80);
            refreshLine();
            printString(cmdBuf);
          }
          break;
        default:
          cmdBuf[cmdIndex] = tempPress;
          printString(cmdBuf + cmdIndex++);
          break;
      }
    }
    /* add current command to history*/
    reorganizeHistory(&history);
    strnCpy(cmdBuf, history[0], cmdIndex);
    if (maxCommands < 19){
      maxCommands++;
    }

    /* end processing input, begin parse arguments*/
    parseArguments(cmdBuf, &argCount, argArray);

    if (strnCmp(argArray[0], "type", 5) == 0 && argCount >= 2) {
      readFile(argArray[1], buffer, &readCount);
      if (readCount > 0) {
        printString(buffer);
        printString("\n\r");
      } else {
        printString("FILE NOT FOUND\n\r");
      }
    } else if (strnCmp(argArray[0], "execute", 8) == 0 && argCount >= 2) {
      params.argc = argCount - 1;
      params.argv = argArray + 1;
      executeProgram(argArray[1], 0, &params);
    } else if (strnCmp(argArray[0], "delete", 7) == 0) {
      deleteFile(argArray[1]);
    } else if (strnCmp(argArray[0], "copy", 5) == 0 && argCount == 3) {
      readFile(argArray[1], buffer, &readCount);
      if (readCount > 0) {
        writeFile(argArray[2], buffer, readCount);
      } else {
        printString("FILE NOT FOUND\n\r");
      }
    } else if (strnCmp(argArray[0], "dir", 4) == 0 && argCount == 1) {
      listDirectory();
    } else if (strnCmp(argArray[0], "create", 7) == 0 && argCount == 2) {
      creatFile(argArray[1]);
    } else if (strnCmp(argArray[0], "clear", 6) == 0) {
      clearScreen();
    } else if (strnCmp(argArray[0], "bgcol", 6) == 0 && argCount >= 2) {
      /* keycode hack, cycles background color */
      interrupt(0x10, 0x0b00, asciiToInt(argArray[1]), 0, 0);
    } else if (strnCmp(argArray[0], "pg2", 4) == 0) {
      interrupt(0x10, 0x0501, 0x80);
    } else if (strnCmp(argArray[0], "pg1", 4) == 0) {
      interrupt(0x10, 0x0500, 0x80);
    } else if (strnCmp(argArray[0], "draw", 5) == 0) {
      drawStuff();
    } else if (strnCmp(argArray[0], "kill", 5) == 0) {
      killProcess(argArray[1][0] - 0x30);
    } else if (strnCmp(argArray[0], "execforeground", 15) == 0) {
      params.argc = argCount - 1;
      params.argv = argArray + 1;
      executeProgram(argArray[1], &pid, &params);
      blockProcess(pid);
    } else {
      printString("Invalid Command\n\r");
    }
    for (i = 0; i < maxCommands; i++){
      printhex(history[i]);
      //printString("\n\r");
    }
  }
}

void reorganizeHistory(char ***buffer){
  int i;
  char *temp;
  temp = buffer[19];
  for (i = 19; i > 0; i--) {
    strnCpy((*buffer)[i-1], (*buffer)[i], strLen((*buffer)[i-1]));
  }
  buffer[0] = temp;
}

void refreshBuffer(char *buffer, int size){
  int i;
  for (i = 0; i < size; i++){
    buffer[i] = 0x0;
  }
  return;
}

void refreshLine(){
  int i;
  printString("\r");
  for (i = 0; i < 79; i++) {
    printString(" ");
  }
  printString("\r");
  printString("SHELL: ");
}

void processInput(char** historyArray, int *currentCommand, int maxCommands, char* cmdBuf){
  int temp;
  getKeypress(&temp);
  refreshBuffer(cmdBuf, 80);
  if (temp == KEY_ARROW_UP){
    /* up */
    refreshLine();

    printhex(*currentCommand);
    printhex(maxCommands);
    if (*currentCommand < maxCommands){
      printString("HEY THERE\0");
      printString(historyArray[*currentCommand]);
      (*currentCommand)++;
    }
    processInput(historyArray, currentCommand, maxCommands, cmdBuf);

  } else if (temp == KEY_ARROW_DOWN){
    /* down */
    /*printhex(temp);*/
    refreshLine();
    printhex(*currentCommand);
    printhex(maxCommands);
    if (*currentCommand > 0){
      printString(historyArray[*currentCommand]);
      (*currentCommand)--;
    }
    processInput(historyArray, currentCommand, maxCommands, cmdBuf);

  } else if ((char)temp == 0x0){
    /* do nothing*/
    processInput(historyArray, currentCommand, maxCommands, cmdBuf);
  }
  else {
    /* only allow access to command history on first keystroke*/
    cmdBuf[0] = (char)temp;
    printString(cmdBuf);
    interrupt(READSTRING, cmdBuf + 1, 0, 0);
  }
}

void parseArguments(char *args, int *argc, char **argv) {
  int i, inArg;
  *argc = 0;
  inArg = 0;


  i = 0;
  while (args[i] != '\0') {
    if (args[i] != ' ') {
      if (!inArg) {
        (argv)[*argc] = args + i;
        (*argc)++;
      }
      inArg = 1;
    } else if (args[i] == ' ') {
      inArg = 0;
      args[i] = 0x0;
    }
    i++;
  }
}

void listDirectory() {
  char dir[512], line[20];
  int i, j, count, charsCopied;

  strnCpy("FILENAME     SIZE\n\r\0", line, 20);

  printString(line);

  readSector(dir, 2);
  for (i = 0; i < 16; i++) {
    count = 0;
    if (dir[32 * i] == 0) {
      continue;
    }
    charsCopied = strnCpy(dir + 32 * i, line, 6);
    if (line[charsCopied - 1] == 0) charsCopied--;
    for (; charsCopied < 13; charsCopied++) {
      line[charsCopied] = ' ';
    }
    j = 6;
    while (dir[32 * i + j] != 0) {
      count++;
      j++;
    }
    line[charsCopied++] = 0x30 + div(count, 10);
    line[charsCopied++] = 0x30 + (count - 10 * div(count, 10));
    line[charsCopied++] = '\n';
    line[charsCopied++] = '\r';
    line[charsCopied] = '\0';
    printString(line);
  }
  printString("\n\r");
}

void creatFile(char *name) {
  char map[512], dir[512], line[80], writeBuffer[13312];
  int i, writeIndex, count;

  readSector(map, 1);
  readSector(dir, 2);

  for (i = 0; i < 13312; i++) {
    writeBuffer[i] = 0;
  }
  /* find available sectors in map*/

  writeIndex = 0;
  while (1) {
    readString(line);
    count = strLen(line);

    if (count == 0) {
      /* write buffer to next sector*/
      writeFile(name, writeBuffer, div(writeIndex, 512) + 1);
      return;
    }
    if (count + writeIndex > 13312) {
      strnCpy(line, writeBuffer + writeIndex, 13312 - writeIndex);
      writeFile(line, writeBuffer, 26);
      printString("File too long, truncating previous line.\n\r\0");
    } else {
      strnCpy(line, writeBuffer + writeIndex, count);
      writeIndex += count;
    }
  }
}

void clearScreen() {
  // int i;
  // for (i = 0; i < 25; i++) {
  //   printString("\n");
  // }
  interrupt(0x10, 0x0003, 0, 0, 0);
}

#define WIDTH 640
#define HEIGHT 200

void drawStuff() {
  int keycode, x, y, size, i, j;
  char color;
  x = 0; y = 0; size = 4;
  color = 1;
  //change video mode to graphics
  interrupt(0x10, 0x000E, 0, 0, 0);
  while (1) {
    getKeypress(&keycode);
    
    switch (keycode) {
    case KEY_ESCAPE:
      goto end;
    case KEY_I:
    case KEY_ARROW_UP: //up arrow
      y -= size;
      if (y < 0) y = 0;
      break;
    case KEY_K:
    case KEY_ARROW_DOWN: //down arrow
      y += size;
      if (y > HEIGHT - size) y = HEIGHT - size;
      break;
    case KEY_J:
    case KEY_ARROW_LEFT: //left arrow
      x -= size * 2;
      if (x < 0) x = 0;
      break;
    case KEY_L:
    case KEY_ARROW_RIGHT: //right arrow
      x += size * 2;
      if (x > WIDTH - size * 2) x = WIDTH - size * 2;
      break;
    case KEY_1: case KEY_KP_1:
    case KEY_2: case KEY_KP_2:
    case KEY_3: case KEY_KP_3:
    case KEY_4: case KEY_KP_4:
    case KEY_5: case KEY_KP_5:
    case KEY_6: case KEY_KP_6:
    case KEY_7: case KEY_KP_7:
    case KEY_8: case KEY_KP_8:
    case KEY_9: case KEY_KP_9:
    case KEY_0: case KEY_KP_0:
      color = KEYCODE_TO_ASCII(keycode) - 0x30;
      break;
    case KEY_A:
    case KEY_B:
    case KEY_C:
    case KEY_D:
    case KEY_E:
    case KEY_F:
      color = keycode - 0x61 + 10;
      break;
    case KEY_MINUS: case KEY_KP_MINUS:
      size >>= 1;
      if (size < 1) size = 1;
      break;
    case KEY_EQUAL: case KEY_KP_PLUS:
      size <<= 1;
      if (size > 8) size = 8;
      break;
    case KEY_SPACE:
      for (i = 0; i < size * 2 && x + i < WIDTH; i++) {
        for (j = 0; j < size && y + j < HEIGHT; j++) {
          interrupt(0x10, 0x0c00 + color, 0, x + i, y + j);
        }
      }
      break;
    default: break;
    }
  }
end:
  //change video mode to text
  interrupt(0x10, 0x0003, 0, 0, 0);
}

void printhex(int a) {
  char arr[5];
  int temp;
  temp = (a & 0xf000) >> 12;
  if(temp > 9) {
    temp = temp - 9 + 0x10;
  }
  arr[0] = 0x30 + temp;

  temp = (a & 0xf00) >> 8;
  if(temp > 9) {
    temp = temp - 9 + 0x10;
  }
  arr[1] = 0x30 + temp;

  temp = (a & 0xf0) >> 4;
  if(temp > 9) {
    temp = temp - 9 + 0x10;
  }
  arr[2] = 0x30 + temp;

  temp = (a & 0xf);
  if(temp > 9) {
    temp = temp - 9 + 0x10;
  }
  arr[3] = 0x30 + temp;

  arr[4] = 0;
  printString(arr);
}
