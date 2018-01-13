/* Written by Chris Nurrenberg, Zac Davidsen, Trey Lewis 1/13/18*/

#define VMEM_START 0xB8000
#define QEMU_OFFSET 80*9*2
#define VMEM_WRITE_ARG1 0xB000
#define VMEM_WRITE_ARG2 0x8000+QEMU_OFFSET
int main() {
  int i;
  char *hello;
  hello = "Hello World!";

  for (i = 0; i < 12; i++) {
    putInMemory(VMEM_WRITE_ARG1, VMEM_WRITE_ARG2 + i*2, hello[i]);
    putInMemory(VMEM_WRITE_ARG1, VMEM_WRITE_ARG2 + i*2 +1, 0x7);
  }

  while (1)
    continue;
}
