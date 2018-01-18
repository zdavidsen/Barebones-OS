/* Written by (Team 02) Chris Nurrenberg, Zac Davidsen, Trey Lewis 1/17/18*/
void printString(char *string) {

char al, ah;
int ax, i;

for (i = 0;; i++){
  
  al = string[i];
  if (al == 0)
    break;
  ah = 0xE;
  ax = ah * 256 + al;
  interrupt(0x10, ax, 0, 0, 0);

  }

}

void readString(char *string){
  char temp, al, ah;
  int ax, i;
  i = 0;
  while(1){
  ax = 0;
  temp = interrupt(0x16, ax, 0, 0, 0);  
  if (temp == 0xd){
    string[i] = 0xa;
    string[i+1] = 0xd;
    string[i+2] = 0x0;
    ax = 0xE * 256 + 0xa;
    interrupt(0x10, ax, 0, 0, 0);
    ax = 0xE * 256 + 0xd;
    interrupt(0x10, ax, 0, 0, 0);
    return;
  } else if (temp == 0x8){
      if ( i > 0 )
        i--;
      ax = 0xE * 256 + 0x8;
      interrupt(0x10, ax, 0, 0, 0);
      ax = 0xE * 256 + ' ';
      interrupt(0x10, ax, 0, 0, 0);
      ax = 0xE * 256 + 0x8;
      interrupt(0x10, ax, 0, 0, 0);
  } else {
      string[i] = temp;
      i++;  
      ax = 0xE * 256 + temp;
      interrupt(0x10, ax, 0, 0, 0);
  }
}
}
