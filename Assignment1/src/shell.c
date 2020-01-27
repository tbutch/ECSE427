#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// File includes
#include "../inc/node.h"
#include "../inc/shell.h"
#include "../inc/interpreter.h"
#include "../inc/shellmemory.h"

// *************************************
// Function prototypes
char * getInput();
void printWelcomeText();
char * getInput();
void initShellMemory(mem_t * shellMemory[]);
void runScriptFromCommandLine(int argc, char ** argv, mem_t * shellMemory[], int shellMemorySize);

// *************************************

// Global Variables
char prompt[USER_LINE_INPUT_SIZE] = {'$', ' '};
char userInput[USER_LINE_INPUT_SIZE];
int status = 0;
//


int main(int argc, char** argv){
    mem_t * shellMemory[SHELL_MEMORY_SIZE];
    initShellMemory(shellMemory);
    printWelcomeText();
    while(1) {
        printf("%s", prompt);
        fgets(userInput, USER_LINE_INPUT_SIZE -1, stdin);

        status = parseAndEvaluate(userInput, shellMemory, SHELL_MEMORY_SIZE, USER_LINE_INPUT_SIZE);
        
        if(status == SUCCESS || status == MALFORMED_COMMAND){
            // Malformed command is handled by the interpreter
            // Success is good!
        } else if(status == QUIT_TERMINAL){
            // graceful exit requested
            printf("Bye!\n");
            freeShellMemory(shellMemory, SHELL_MEMORY_SIZE);
            return 0;
        }
    }   
}

void printWelcomeText(){
    printf("Welcome to the Tristan Bouchard shell!\n");
    printf("Version 1.0 Created January 2020\n");
}

void initShellMemory(mem_t * shellMemory[]){
    char nullVal[] = "\0";
    for(int i = 0; i < SHELL_MEMORY_SIZE; i++){
        shellMemory[i] = malloc(sizeof(mem_t));
        shellMemory[i]->value = strdup(nullVal);
        shellMemory[i]->var = strdup(nullVal);
    }
}
