#ifndef RAM
#define RAM

#define RAMSIZE 1000
// Ram memory;
#include <stdbool.h>

char * ram[RAMSIZE];

// public API
bool addToRAM(FILE * p , int * start, int * end);
bool cleanRam();
bool initRam();
int getNextAvailableLineInRam();
//bool cleanRamLines(int start, int end);

#endif
