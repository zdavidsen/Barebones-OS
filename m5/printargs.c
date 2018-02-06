
#include "syscall.h"

void printhex(int a);

int main(int argc, char *argv[]) {
  char *temp;
  int i;
  enableInterrupts();
  printString("Printing arguments:\n\r");
  temp = "argc: 0\n\r";
  temp[6] += argc;
  printString(temp);

  temp = "argv[0]: ";

  for (i = 0; i < argc; i++) {
    temp[5] = 0x30 + i;
    printString(temp);
    printString(argv[i]);
    printString("\n\r");
  }
  // printhex(&argv);
  // for (argc = 0; argc < 0xe5; argc++) {
  //   printhex(*((char*)(&argc) + argc));
  //   printString("    ");
  //   printhex((char*)(&argc) + argc);
  //   getKeypress(&temp);
  //   printString("\n\r");
  // }
  terminate();
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