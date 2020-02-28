#ifndef KERNEL
#define KERNEL

#include <stdbool.h>
// Public function prototypes
int myInit(char* fileName);
int scheduler(mem_t * shellMemory[], int shellMemoryMaxSize, int maxInputSize);

#endif