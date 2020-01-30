/**
 * shellmemory header file. This file contains the publicly available functions
 * used to manipulate the shell memory. Needs to be included in order to use
 * shellmemory.c
 * 
 * Author: Tristan Bouchard
 * Date: January 29, 2020
 * 
 */

#ifndef SHELL_MEM
#define SHELL_MEM

// Typedef'ed struct to not have to write struct all over the place
typedef struct{
    char * var;
    char * value;
} mem_t;

// Public functions
mem_t * makeNewMemoryNode(char * varName, char * varValue);
int addMemoryNodeToShellMem(mem_t * shellMemory[], int maxMemSize, char * wordsArray[], int lengthOfInput);
int variableExistsInMemory(mem_t * shellMemory[], char * varName, int maxMemSize);
int getVariablePositionInMemoryArray(mem_t * shellMemory[], char * varName, int maxMemSize);
int freeShellMemory(mem_t * shellMemory[], int maxMemSize);
int overwriteVariableInMemory(mem_t * shellMemory[], int maxMemSize, char * wordsArray[], int wordCount);

#endif