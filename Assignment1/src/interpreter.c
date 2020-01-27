#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// File includes
#include "../inc/interpreter.h"

//Function prototypes
int isEqual(char str1[], char str2[]);
void unknownCommand();
int getLengthOfInput(char * wordsArray[], int shellMemoryMaxSize);
int setVariableToShellMemory(char * wordsArray[], mem_t * shellMemory[], int shellMemoryMaxSize);
int printShellVariable(char * wordsArray[], mem_t * shellMemory[], int shellMemoryMaxSize);
void printHelpScreen();
int runScript(char * wordArray[], mem_t * shellMemory[], int shellMemoryMaxSize, int maxInputSize);
int parseInput(char * wordArray[], mem_t * shellMemory[], int shellMemoryMaxSize, int maxInputSize);

/**
 * 
 * 
 */
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
    char * wordArray[maxInputSize]; // Word array, of length 100

    // Parse input string while not null
    while(input[i] != '\0'){
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
                // just skip it!
            }
            
        } 
        // if end of string
        else if(currentChar == '\n'){
            currWord[currWordLength] = '\0';
            wordArray[wordCount] = strdup(currWord);
            strcpy(currWord,"");
            wordCount++;
            break;
        }
        else {
            currWord[currWordLength] = currentChar;
            currWordLength++;
        }
        i++;
    }
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
        // Compare
        // Set cmd to lowercase - Not required for this assignment
        // for(int i = 0; wordArray[0][i]; i++){
        //     wordArray[0][i] = tolower(wordArray[0][i]);
        // }
        if(isEqual(wordArray[0], (char *) helpCommand)){
            printHelpScreen();
        }
        else if(isEqual(wordArray[0], (char *) quitCommand)){
            return QUIT_TERMINAL;
        }
        else if(isEqual(wordArray[0], (char *) setCommand)){
            errorCode = setVariableToShellMemory(wordArray, shellMemory, shellMemoryMaxSize);
        }
        else if(isEqual(wordArray[0], (char *) printCommand)){
            errorCode = printShellVariable(wordArray, shellMemory, shellMemoryMaxSize);
        }
        else if(isEqual(wordArray[0], (char *) runCommand)){
            errorCode = runScript(wordArray, shellMemory, shellMemoryMaxSize, maxInputSize);
        }
        else {
            unknownCommand();
            return UNKOWN_COMMAND;
        }

        // Verify the errorCode
        if(errorCode == MALFORMED_COMMAND){
            // Malformed command
            printf("Malformed %s command\n", wordArray[0]);
            return MALFORMED_COMMAND;
        } else if(errorCode == FAILURE){
            // some sort of error
        } else if (errorCode == SUCCESS){
            // All good!
        } else if(errorCode == QUIT_TERMINAL) {
            printf("Bye!\n");
        }
    }
    return SUCCESS;
}

/*
 * Function: isEqual
 * -----------------------------------------------------------------------
 *  Function used to compare two strings more inuitivel
 * 
 *  Returns: 1 if equal, 0 if not
 */
int isEqual(char str1[], char str2[]){
    if( strcmp(str1,str2)==0 ){
        return 1;
    } else {
        return 0;
    }
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
int getLengthOfInput(char * wordsArray[], int shellMemoryMaxSize){
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
    int wordCount = getLengthOfInput(wordsArray, shellMemoryMaxSize);
    if(wordCount < 3){
        // Malformed command
        return MALFORMED_COMMAND;
    }
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
 *           0 - On failure,
 *           1 - On success 
 *           3 - On malformed command.
 */
int printShellVariable(char * wordsArray[], mem_t * shellMemory[], int shellMemoryMaxSize){
    int inputLength = getLengthOfInput(wordsArray, shellMemoryMaxSize);
    if(inputLength != 2){
        // Malformed command
        return MALFORMED_COMMAND;
    }
    int variableIndex = getVariablePositionInMemoryArray(shellMemory, wordsArray[1], shellMemoryMaxSize);
    if(variableIndex == -1){
        // Variable does not exist
        printf("Variable does not exist\n");
        return SUCCESS;
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
    int lengthOfInput = getLengthOfInput(wordArray, shellMemoryMaxSize);
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
