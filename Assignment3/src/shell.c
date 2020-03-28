/**
 * Shell program file of the Tristan Bouchard shell, which contains the shellUI() function
 * called by the kernel.
 * All assumtions taken when programming this assignment are available in the 
 * README.md file in the Assignment1 directory
 * 
 * Author: Tristan Bouchard
 * Date: January 29 2020
 *      
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// File includes
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

/*
 * Public function: main(char argc, cha* argc[])
 * -----------------------------------------------------------------------
 *  Main program entry
 * 
 *  Returns: 0 if successful, 1 if not.
 */
int shellUI(int argc, char** argv){
    mem_t * shellMemory[SHELL_MEMORY_SIZE];
    initShellMemory(shellMemory);
    printWelcomeText();
    while(1) {
        printf("%s", prompt);
        
        // This if statement handles EOF.
        if(fgets(userInput, USER_LINE_INPUT_SIZE -1, stdin) == NULL){
            break;
        }

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

/*
 * Private function: printWelcomeText()
 * -----------------------------------------------------------------------
 *  Prints the welcome text when the terminal is opened
 * 
 *  Returns: void
 */
void printWelcomeText(){
    printf("Kernel 2.0 loaded!\n");
    printf("Welcome to the Tristan Bouchard shell!\n");
    printf("Version 3.0 Updated March 2020\n");
}

/*
 * Private function: addMemoryNodeToShellMem
 * -----------------------------------------------------------------------
 *  Function used to initialize the shell memory to null values
 * 
 *  Returns: 1 if successful, 0 if not.
 */
void initShellMemory(mem_t * shellMemory[]){
    char nullVal[] = "\0";
    for(int i = 0; i < SHELL_MEMORY_SIZE; i++){
        // Use the strdup function here to duplicate the local string and 
        // return the address of the duplicated string.
        shellMemory[i] = malloc(sizeof(mem_t));
        shellMemory[i]->value = strdup(nullVal);
        shellMemory[i]->var = strdup(nullVal);
    }
}
