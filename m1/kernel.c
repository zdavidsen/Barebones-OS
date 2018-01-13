/* Written by Chris Nurrenberg, Zac Davidsen, Trey Lewis 1/13/18*/

#define VMEM_START 0xB8000
#define QEMU_OFFSET 80*9*2
#define VMEM_WRITE_ARG1 0xB000
#define VMEM_WRITE_ARG2 0x8000+QEMU_OFFSET
int main(){
  int i;
  char hello[13];
  hello[0] = 'H';
  hello[1] = 'e';
  hello[2] = 'l';
  hello[3] = 'l';
  hello[4] = 'o';
  hello[5] = ' ';
  hello[6] = 'W';
  hello[7] = 'o';
  hello[8] = 'r';
  hello[9] = 'l';
  hello[10] = 'd';
  hello[11] = '!';
   
  /*char hello[13] = {'H', 'e', 'l', lo World!";*/
  for (i = 0; i < 12; i++){
    putInMemory(VMEM_WRITE_ARG1, VMEM_WRITE_ARG2 + i*2, hello[i]);
    putInMemory(VMEM_WRITE_ARG1, VMEM_WRITE_ARG2 + i*2 +1, 0x7);
  }
  while(1);
}
