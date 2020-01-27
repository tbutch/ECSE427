/**
 * ShellMemory.c file. This file contains functions to manipulate the shellMemory array
 * specified in the function arguments. It assumes that the array is well formed, without
 * any holes in the array (aka missing values).
 * 
 * Author: Tristan Bouchard
 * Date: January 2020
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// File includes
#include "../inc/shellmemory.h"
#include "../inc/interpreter.h"

// Function prototypes 
int addMemoryNodeToShellMemAtIndex(mem_t * shellMemory[], int maxMemSize, char * wordsArray[], int index, int lengthOfInput);

/*
 * Private function: freeMemoryNode
 * -----------------------------------------------------------------------
 *  Function to free the specified memory nodes
 * 
 *  Returns: 1 if successful, 0 if not.
 */
int freeMemoryNode(mem_t * node){
    // free memory allocated to strings
    if(node == NULL){
        return 0;
    }
    if(node->value != NULL){
        free(node->value);
    }
    if(node->var != NULL){
        free(node->var);
    }
    free(node);
    return 1;
}

void clearMemoryNode(mem_t * node){
    // TODO: Implement this to remove a variable from the list.
    // Also should re-organize the list such that there are no "holes"
}

/*
 * Public function: addMemoryNodeToShellMem
 * -----------------------------------------------------------------------
 *  Add variable to the end of the shellMemory.
 * 
 *  Returns: 1 if successful, 0 if not.
 */
int addMemoryNodeToShellMem(mem_t * shellMemory[], int maxMemSize, char * wordsArray[], int lengthOfInput){
    // Find next available node
    int i = 0;
    while(i <= maxMemSize){
        if(i == maxMemSize){
            break;
        }
        else if(isEqual(shellMemory[i]->var, "\0")){
            break;
        }
        i++;
    }
    if(i == maxMemSize){
        printf("Shell memory is now full, cannot add variable\n");
        return FAILURE;
    }
    // Concatenate the rest of the available arguments into one string
    return addMemoryNodeToShellMemAtIndex(shellMemory, maxMemSize, wordsArray, i, lengthOfInput);
}

/*
 * Private function: addMemoryNodeToShellMemAtIndex
 * -----------------------------------------------------------------------
 *  Add variable to the end of the shellMemory.
 * 
 *  Returns: 1 if successful, 0 if not.
 */
int addMemoryNodeToShellMemAtIndex(mem_t * shellMemory[], int maxMemSize, char * wordsArray[], int index, int lengthOfInput){
    // char str[1000] = "";
    // for(int w = 2; w < lengthOfInput; w++){
    //     strcat(str, wordsArray[w]);
    //     if(w != lengthOfInput -1){
    //         strcat(str, " ");
    //     }
    // }
    char * str = wordsArray[2];
    free(shellMemory[index]->var);
    free(shellMemory[index]->value);
    //shellMemory[index]->var = malloc((strlen(wordsArray[1]) + 1) * sizeof(char));
    //shellMemory[index]->value = malloc((strlen(str) + 1) * sizeof(char));
    shellMemory[index]->var = strdup(wordsArray[1]);
    shellMemory[index]->value = strdup(str);
    return SUCCESS;
}

/*
 * Public function: variableExistsInMemory
 * -----------------------------------------------------------------------
 *  Description: Function used to verify that a variable with the specified
 *  name exists in the shellMemory
 * 
 *  Returns: 1 if the variable exists, zero otherwise 
 */
int variableExistsInMemory(mem_t * shellMemory[], char * varName, int maxMemSize){
    int i = 0;
    while(i <= maxMemSize){
        if(i == maxMemSize){
            return 0;
        }
        else if(!isEqual(shellMemory[i]->var, "\0")){
            if(isEqual(shellMemory[i]->var, varName)){
                return 1;
            }
        }
        i++;
    }
    return 0;
}

/*
 * Public function: getVariablePositionInMemoryArray
 * -----------------------------------------------------------------------
 *  Function used to obtain the position of the specified variable in the
 *  shellMemory array.
 * 
 *  Returns: index of the specified variable if found, -1 otherwise.
 */
int getVariablePositionInMemoryArray(mem_t * shellMemory[], char * varName, int maxMemSize){
    int i = 0;
    while(i <= maxMemSize){
        if(i == maxMemSize){
            return -1;
        } else if(!isEqual(shellMemory[i]->var, "\0")){
            if(isEqual(shellMemory[i]->var, varName)){
                return i;
            }
        }
        i++;
    }

    return -1;
}

/*
 * Public function: freeShellMemory
 * -----------------------------------------------------------------------
 *  Function used to free the specified shellMemory 
 * 
 *  Returns: 0 on failure, 1 on success
 */
int freeShellMemory(mem_t * shellMemory[], int maxMemSize){
    int i = 0;
    for(int i = 0; i < maxMemSize; i++){
        int success = freeMemoryNode(shellMemory[i]);
        if(!success){
            return 0;
        }
        i++;
    }
    return 1;
}

/*
 * Public function: overwriteVariableInMemory
 * -----------------------------------------------------------------------
 *  Function used overwrite the value of a variable in the shell memory
 * 
 *  Returns: 0 on failure, 1 on success
 */
int overwriteVariableInMemory(mem_t * shellMemory[], int maxMemSize, char * wordsArray[], int wordCount){
    int variableIndex = getVariablePositionInMemoryArray(shellMemory, wordsArray[1], maxMemSize);
    if(variableIndex == -1 || variableIndex >= maxMemSize){
        return FAILURE;
    }
    return addMemoryNodeToShellMemAtIndex(shellMemory, maxMemSize, wordsArray, variableIndex, wordCount);
}