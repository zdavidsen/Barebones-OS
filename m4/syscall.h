#define PRINTSTRING 0x21, 0
#define READSTRING 0x21, 1
#define READSECTOR 0x21, 2
#define READFILE 0x21, 3
#define EXECUTEPROGRAM 0x21, 4
#define TERMINATE 0x21, 5
#define WRITESECTOR 0x21, 6
#define DELETEFILE 0x21, 7
#define WRITEFILE 0x21, 8

#define CLRSCREEN 0x21, 10

#define printString(string) interrupt(PRINTSTRING, string, 0, 0)
#define readString(buffer) interrupt(READSTRING, buffer, 0, 0)
#define readSector(buffer, sector) interrupt(READSECTOR, buffer, sector, 0)
#define readFile(name, buffer, count) interrupt(READFILE, name, buffer, count)
#define executeProgram(data, sector) interrupt(EXECUTEPROGRAM, data, sector, 0)
#define terminate() interrupt(TERMINATE, 0, 0, 0)
#define writeSector(data, sector) interrupt(WRITESECTOR, data, sector, 0)
#define deleteFile(name) interrupt(DELETEFILE, name, 0, 0)
#define writeFile(name, data, numSectors) interrupt(WRITEFILE, name, data, numSectors)

#define clearScreen() interrupt(CLRSCREEN, 0, 0, 0)