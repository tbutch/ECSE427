#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// File includes
#include "../inc/ram.h"
#include "../inc/shell.h"
#include "../inc/stringUtilities.h"

/*
 * Function: addToRAM
 * -----------------------------------------------------------------------
 *  Function used to add a file to the RAM memory.
 * 
 *  Returns: true if success, as well as start and end lines of the program
 *           as stored in memory, false otherwise.
 */
bool addToRAM(FILE *p, int *start, int *end){
    // Step 2: find the next available cell in ram[]
    int nextLine = getNextAvailableLineInRam();
    if(nextLine == -1){
        // RAM is already full
        return false;
    }

    *start = nextLine;
    char buffer[USER_LINE_INPUT_SIZE];
    // fgets(userInput, USER_LINE_INPUT_SIZE -1, stdin)
    while(fgets(buffer, USER_LINE_INPUT_SIZE, p)){
        // ignore newlines
        if(isEqual(buffer,"\n")){
            continue;
        }
        if(nextLine == RAMSIZE){
            // RAM full, but program not completely copied!
            return false;
        }
        // Step 3: copy all the lines of code into ram[]
        ram[nextLine] = strdup(buffer);
        nextLine++;
    }
    // Step 4: remember the start cell number and the ending cell number of that script in ram[]
    *end = nextLine - 1;
    return true;
}

bool cleanRam(){
    for(int i = 0; i < RAMSIZE; i++){
        ram[i] = NULL;
    }
    return true;
}
/*
 * Function: initRam
 * -----------------------------------------------------------------------
 *  Function used to initialize the RAM memory
 * 
 *  Returns: true on success, false error;
 */
bool initRam(){
    return cleanRam();
}

/*
 * Function: getNextAvailableLineInRam
 * -----------------------------------------------------------------------
 *  Function used to get next free line in RAM
 * 
 *  Returns: next free line on success, -1 on error;
 */
int getNextAvailableLineInRam(){
    int i;
    for(i = 0; i < RAMSIZE; i++){
        if(ram[i] == NULL){
            return i;
        }
    }
    // RAM full!
    return -1;
}

/*
 * Function: getNextAvailableLineInRam
 * -----------------------------------------------------------------------
 *  Function used to get next free line in RAM
 * 
 *  Returns: next free line on success, -1 on error;
 */
bool cleanRamLines(int start, int end){
    if(start < 0 || end >= RAMSIZE){
        return false;
    }
    for(int i = start; i <= end; i++){
        ram[i] = NULL;
    }
}


