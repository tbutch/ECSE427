/**
 * CPU header file. Here, the CPU struct is defined and the available
 * API is described.
 * 
 * Author: Tristan Bouchard
 * Date: April 29, 2020
 * 
 */

#ifndef CPU
#define CPU

#include "../inc/shell.h"
#include "../inc/shellmemory.h"

typedef struct{
    int IP;
    int offset;
    char IR[USER_LINE_INPUT_SIZE];
    int quanta;
} CPU_t;

// Constants
#define BASE_QUANTA 2
#define CPU_START_ADDRESS 0

CPU_t * cpu;
// Public function prototypes
CPU_t * initCPU();
void disposeCPU();
int run(int quanta, mem_t * shellMemory[], int shellMemoryMaxSize, int maxInputSize);
CPU_t * resetCPU();

#endif
