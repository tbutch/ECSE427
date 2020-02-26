#ifndef CPU
#define CPU

#include "../inc/shell.h"

typedef struct{
    int IP;
    char IR[USER_LINE_INPUT_SIZE];
    int quanta;
} CPU_t;

// Public function prototypes
int initCPU(CPU_t * cpu);
void run(int quanta);

#endif
