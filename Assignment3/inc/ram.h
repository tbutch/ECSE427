#ifndef RAM
#define RAM

// Total number of frames in RAM
#define NUMBER_OF_FRAMES 10
// Number of strings contained by a RAM frame
#define FRAME_SIZE 4
#define RAMSIZE (NUMBER_OF_FRAMES * FRAME_SIZE)
// Ram memory;
#include <stdbool.h>

// RAM is 
char * ram[RAMSIZE];

// public API
bool addToRAM(FILE * p , int * start, int * end);
bool cleanRam();
bool initRam();
int getNextAvailableLineInRam();
//bool cleanRamLines(int start, int end);

#endif
