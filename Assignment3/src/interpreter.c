/**
 * interpreter.c file. This file contains functions to interpret the given commands from 
 * the command prompt. Most of the code to this assignment can be found here, as it contains
 * not only the functions to run the commands but also most of the helper functions.
 * 
 * Author: Tristan Bouchard
 * Date: January 29 2020
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// File includes
#include "../inc/interpreter.h"
#include "../inc/shell.h"
#include "../inc/ram.h"
#include "../inc/pcb.h"
#include "../inc/cpu.h"
#include "../inc/stringUtilities.h"
#include "../inc/kernel.h"

// Function prototypes
void unknownCommand();
int getLengthOfInput(char * wordsArray[]);
int setVariableToShellMemory(char * wordsArray[], mem_t * shellMemory[], int shellMemoryMaxSize);
int printShellVariable(char * wordsArray[], mem_t * shellMemory[], int shellMemoryMaxSize);
void printHelpScreen();
int runScript(char * wordArray[], mem_t * shellMemory[], int shellMemoryMaxSize, int maxInputSize);
int parseInput(char * wordArray[], mem_t * shellMemory[], int shellMemoryMaxSize, int maxInputSize);
void wipeWords(char * wordArray[], int arrayLength);
int exec(char * wordArray[], mem_t * shellMemory[], int shellMemoryMaxSize, int maxInputSize);

// Define global array to avoid segfault lol
char * wordArray[SHELL_MEMORY_SIZE];

/*
 * Function: tokenize
 * -----------------------------------------------------------------------
 *  Function used to parse user input. Uses the user input
 *  array and tokenizes it into words in a 2D array, which
 *  is passed to the interpreter.
 * 
 *  Returns: Whatever the parser returns
 */
int parseAndEvaluate(char input[], mem_t * shellMemory[], int shellMemoryMaxSize, int maxInputSize){
    
    int i = 0;
    int wordCount = 0;
    int currWordLength = 0;
    
    char currWord[maxInputSize];
    // make all values null in words array
    wipeWords(wordArray, shellMemoryMaxSize);
    
    // Parse input string while not null
    while(true){
        char currentChar = input[i];

        // If the currentChar is not a space, add it to the 
        // current string
        if(currentChar == ' '){
            // If it is a space, add a null to the end of the current
            // word and add it to the word array (if there is one)
            if(currWordLength > 0){
                currWord[currWordLength] = '\0';
                wordArray[wordCount] = strdup(currWord);
                strcpy(currWord,"");
                currWordLength = 0;
                wordCount++;
            } else {
                // just skip it! This solves inputs with lots of spaces between commands
            }
            
        } 
        // if end of string, with or without newline!
        else if(currentChar == '\n' || currentChar == '\0'){
            currWord[currWordLength] = '\0';
            wordArray[wordCount] = strdup(currWord);
            strcpy(currWord,"");
            wordCount++;
            break;
        }
        // else if(currentChar == '\0'){
        //     currWord[currWordLength] = '\0';
        //     wordArray[wordCount] = strdup(currWord);
        //     strcpy(currWord,"");
        //     wordCount++;
        //     break;
        // }
        else {
            // Add the current character to the current work and increment the length
            // of the current word
            currWord[currWordLength] = currentChar;
            currWordLength++;
        }
        i++;
    }
    // Add a null word at the end of the word array to mark the end
    strcpy(currWord,"\0");
    wordArray[wordCount] = strdup(currWord); 
    return parseInput(wordArray, shellMemory, shellMemoryMaxSize, maxInputSize);
}

/*
 * Function: parseInput
 * -----------------------------------------------------------------------
 * Function used to parse the tokenized input to determine
 * what commands are to be performed
 * 
 *  Returns: 
 *          -1 on fatal error
 *           0 to keep displaying the shell
 *           1 when quit is requested
 *           2 on unknown command
 *           3 on existing but malformed command
 *          
 */
int parseInput(char * wordArray[], mem_t * shellMemory[], int shellMemoryMaxSize, int maxInputSize){
    int i = 0;
    int errorCode;

    // If first word is null, then we have a problem lol
    if(isEqual(wordArray[0], "\0")){
        return FATAL_ERROR;
    }
    else{
        
        // Set cmd to lowercase - Not required for this assignment
        // for(int i = 0; wordArray[0][i]; i++){
        //     wordArray[0][i] = tolower(wordArray[0][i]);
        // }

        // Compare
        // Here, we cast to (char *) because cont char *  != char *
        if(isEqual(wordArray[0], (char *) HELP_COMMAND)){
            printHelpScreen();
        }
        else if(isEqual(wordArray[0], (char *) QUIT_COMMAND) || isEqual(wordArray[0], (char *) EXIT_COMMAND)){
            return QUIT_TERMINAL;
        }
        else if(isEqual(wordArray[0], (char *) SET_COMMAND)){
            errorCode = setVariableToShellMemory(wordArray, shellMemory, shellMemoryMaxSize);
        }
        else if(isEqual(wordArray[0], (char *) PRINT_COMMAND)){
            errorCode = printShellVariable(wordArray, shellMemory, shellMemoryMaxSize);
        }
        else if(isEqual(wordArray[0], (char *) RUN_COMMAND)){
            errorCode = runScript(wordArray, shellMemory, shellMemoryMaxSize, maxInputSize);
        } else if(isEqual(wordArray[0], (char *) EXEC_COMMAND)){
            errorCode = exec(wordArray, shellMemory, shellMemoryMaxSize, maxInputSize);
        }
        else {
            unknownCommand();
            return UNKOWN_COMMAND;
        }

        // Verify the errorCode if not already handled by the command
        if(errorCode == MALFORMED_COMMAND){
            // Malformed command
            printf("Malformed %s command\n", wordArray[0]);
            if(isEqual(wordArray[0], (char *) SET_COMMAND)){
                printf("Command was: %s %s %s", wordArray[0], wordArray[1], wordArray[2]);
            }
            return MALFORMED_COMMAND;
        } else if(errorCode == FAILURE){
            // some sort of error
        } else if (errorCode == SUCCESS){
            // All good!
            return SUCCESS;
        } else if(errorCode == QUIT_TERMINAL) {
            // This section is used when running scripts
            printf("Bye!\n");
        } else if(errorCode == FATAL_ERROR){
            printf("Fatal error occured in %s command\n", wordArray[0]);
        } else if(errorCode == NONEXISTANT_FILE){
            printf("File nonexistent. Simulation aborted\n");
        } else if(errorCode == RAM_LOAD_FAIL){
            printf("Failure to load in RAM. Programs may be too long for memory.\nSimulation aborted\n");
        }
    }
    return SUCCESS;
}

/*
 * Function: unknownCommand
 * -----------------------------------------------------------------------
 *  Function used to print unknown command
 * 
 *  Returns: void
 */
void unknownCommand(){
    printf("Unknown command\n");
}

/*
 * Function: getLengthOfInput
 * -----------------------------------------------------------------------
 *  Function used to get the length in words of the input
 * 
 *  Returns: int size of word array
 */
int getLengthOfInput(char * wordsArray[]){
    int i = 0;
    while(!isEqual(wordsArray[i],"")){
        i++;
    }
    return i;
}

/*
 * Function: setVariableToShellMemory
 * -----------------------------------------------------------------------
 *  Function used to set specified variable to shell memory
 * 
 *  Returns: 
 *           0 - On failure
 *           1 - On success
 *           3 - On malformed command
 */
int setVariableToShellMemory(char * wordsArray[], mem_t * shellMemory[], int shellMemoryMaxSize){
    int wordCount = getLengthOfInput(wordsArray);
    if(wordCount < 3){
        // Malformed command
        return MALFORMED_COMMAND;
    }
    // Verify if variable already exists in shell memory, if not (and not full) add it, if
    // it is, replace its value with the newly specified one
    if(!variableExistsInMemory(shellMemory, wordsArray[1], shellMemoryMaxSize)){
        return addMemoryNodeToShellMem(shellMemory, shellMemoryMaxSize, wordsArray, wordCount);
    } else {
        return overwriteVariableInMemory(shellMemory, shellMemoryMaxSize, wordsArray, wordCount);
    }
}

/*
 * Function: printShellVariable
 * -----------------------------------------------------------------------
 *  Function used print specified variable on the shell, otherwise print
 *  "Variable does not exist".
 * 
 *  Returns: 
 *           1 - On success 
 *           3 - On malformed command.
 */
int printShellVariable(char * wordsArray[], mem_t * shellMemory[], int shellMemoryMaxSize){
    int inputLength = getLengthOfInput(wordsArray);
    if(inputLength != 2){
        // Malformed command
        return MALFORMED_COMMAND;
    }
    int variableIndex = getVariablePositionInMemoryArray(shellMemory, wordsArray[1], shellMemoryMaxSize);
    if(variableIndex == -1){
        // Variable does not exist
        printf("Variable does not exist!\n");
        return(SUCCESS);
    }
    printf("%s\n", shellMemory[variableIndex]->value);
    return SUCCESS;
}

/*
 * Function: printHelpScreen
 * -----------------------------------------------------------------------
 *  Function used print help screen when "help" command is called
 * 
 *  Returns: void
 */
void printHelpScreen(){
    printf("Supported commands are the following:\n\n");
    printf("\thelp:  Command used to display this help screen.\n\n");
    printf("\tquit:  Command used to exit the shell.\n\n");
    printf("\tset:   Command used to set variable and value to shell memory.\n");
    printf("\t       Command format: set VARIABLE VALUE\n");
    printf("\t       Example command: set mary 10\n\n");
    printf("\tprint: Command used to print variable existing in shell memory.\n");
    printf("\t       Command format: print VARIABLE\n");
    printf("\t       Example command: print mary\n\n");
    printf("\trun:   Command used to run specified text file script with supported commands.\n");
    printf("\t       Command format: run SCRIPT.txt\n\n");
    printf("\texec:  Command used to run specified scripts in a parallel CPU simulation.\n");
    printf("\t       Command format: exec SCRIPT1.txt [SCRIPT2.txt] [SCRIPT3.txt]\n\n");
    
    printf("More supported commands to come!\n");
}

/*
 * Function: runScript
 * -----------------------------------------------------------------------
 *  Function used to run the specified script
 * 
 *  Returns: 1 on success, -1 on nonexistent file, 0 error;
 */
int runScript(char * wordArray[], mem_t * shellMemory[], int shellMemoryMaxSize, int maxInputSize){
    char * fileName = wordArray[1];
    int lengthOfInput = getLengthOfInput(wordArray);
    if(lengthOfInput < 2){
        return MALFORMED_COMMAND;
    }
    FILE * file = fopen(fileName, "r");
    if(file == NULL){
        printf("Error opening file %s\n", fileName);
        return FATAL_ERROR;
    }
    char currLine[shellMemoryMaxSize];
    int status;
    while(fgets(currLine, shellMemoryMaxSize-1, file)){
        status = parseAndEvaluate(currLine, shellMemory, shellMemoryMaxSize, maxInputSize);
        if(status == QUIT_TERMINAL){
            // Quit command was entered
            break;
        }
        // else if (status == MALFORMED_COMMAND || status == UNKOWN_COMMAND){
        //     // Unknown command, terminate script execution
        //     printf("---------------------------------------------------------------\n");
        //     printf("    Unknown command detected - Terminating script execution\n");
        //     printf("---------------------------------------------------------------\n");
        //     break;
        // }
    }
    fclose(file);
    return status;
}

/*
 * Function: execCommand
 * -----------------------------------------------------------------------
 *  Function used to run the specified scripts concurrently. Maximum of
 *  3 scripts, must all be unique (no duplicates)
 * 
 *  Returns: 1 on success, -1 on nonexistent file, 0 error;
 */
int exec(char * wordArray[], mem_t * shellMemory[], int shellMemoryMaxSize, int maxInputSize){
    // Input validation
    int inputLength = getLengthOfInput(wordArray);

    if(inputLength > 4 || inputLength < 2){
        return MALFORMED_COMMAND;
    }
    bool sameCommands = false;
    switch (inputLength)
    {
    case 2:
        sameCommands = false;
    case 3:
        sameCommands = (bool) isEqual(wordArray[1], wordArray[2]);
        break;
    case 4:
        sameCommands = (bool) isEqual(wordArray[1], wordArray[2]) || (bool) isEqual(wordArray[1], wordArray[3]) || (bool) isEqual(wordArray[2], wordArray[3]);
        break;
    default:
        return FATAL_ERROR;
    }
    if(sameCommands){
        printf("Error: Script is already loaded.\n");
        return MALFORMED_COMMAND;
    }

    for(int i = 1; i < inputLength; i++){
        int status = myInit(wordArray[i]);

        if(status == NONEXISTANT_FILE || status == RAM_LOAD_FAIL){
            cleanRam();
            return status;
        }
    }
    return scheduler(shellMemory, shellMemoryMaxSize, maxInputSize);
}

/*
 * Function: wipeWords 
 * -----------------------------------------------------------------------
 *  Function used to wipe the global words array.
 * 
 *  Returns: void
 */
void wipeWords(char * wordArray[], int arrayLength){
    for(int w = 0; w < arrayLength; w++){
        wordArray[w] = strdup("\0");
    }
}
