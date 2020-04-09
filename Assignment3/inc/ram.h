/**
 * RAM header file. Here, constants and available functions provided by the RAM.c API 
 * can be found and used.
 * 
 * Author: Tristan Bouchard
 * Date: April 29, 2020
 * 
 */

#ifndef RAM
#define RAM

// Total number of frames in RAM
#define NUMBER_OF_FRAMES 10
// Number of strings contained by a RAM frame
#define FRAME_SIZE 4
#define RAMSIZE (NUMBER_OF_FRAMES * FRAME_SIZE)
// Ram memory;
#include <stdbool.h>
#include "../inc/pcb.h"

// RAM is 
char * ram[RAMSIZE];

// public API
bool cleanRam();
bool initRam();
int getNextAvailableLineInRam();
bool addPageToRAM(FILE * file, PCB_t * pcb, int pageNumber, int frameNumber);
//bool cleanRamLines(int start, int end);

#endif
