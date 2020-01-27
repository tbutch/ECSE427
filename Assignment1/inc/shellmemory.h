#ifndef SHELL_MEM
#define SHELL_MEM

typedef struct{
    char * var;
    char * value;
} mem_t;

mem_t * makeNewMemoryNode(char * varName, char * varValue);
int addMemoryNodeToShellMem(mem_t * shellMemory[], int maxMemSize, char * wordsArray[], int lengthOfInput);
int variableExistsInMemory(mem_t * shellMemory[], char * varName, int maxMemSize);
int getVariablePositionInMemoryArray(mem_t * shellMemory[], char * varName, int maxMemSize);
int freeShellMemory(mem_t * shellMemory[], int maxMemSize);
int overwriteVariableInMemory(mem_t * shellMemory[], int maxMemSize, char * wordsArray[], int wordCount);

#endif