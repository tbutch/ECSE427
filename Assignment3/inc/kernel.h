/**
 * kernel header file. Here, some constants used by the kernel are defined
 * as well as certain public functions available as an API.
 * 
 * Author: Tristan Bouchard
 * Date: April 29, 2020
 * 
 */

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
int prepareBackingStore();

#endif