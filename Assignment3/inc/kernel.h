#ifndef KERNEL
#define KERNEL

#include <stdbool.h>
#include "../inc/shellmemory.h"

// Public constants
static const char BACKING_STORE_NAME[] = "./BackingStore/";
static const size_t MAX_PATH_SIZE = 100;

// Public function prototypes
int myInit(char* fileName);
int scheduler(mem_t * shellMemory[], int shellMemoryMaxSize, int maxInputSize);

#endif