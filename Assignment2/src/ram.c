#include <stdio.h>
#include <stdlib.h>

// File includes
#include "../inc/ram.h"

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
    // Step 3: copy all the lines of code into ram[]
    // Step 4: remember the start cell number and the ending cell number of that script in ram[]
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


