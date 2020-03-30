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

/*
 * Function: addFrameToRAM
 * -----------------------------------------------------------------------
 *  Function used to add a specified page from the backing store to RAM
 * 
 *  Returns: true if success, false otherwise
 */
bool addPageToRAM(FILE * file, PCB_t * pcb, int pageNumber, int frameNumber){
    rewind(file);
    int startLine = pageNumber * FRAME_SIZE;
    int offset = 0;
    // Max page and max line include zero as a number, so a file with lines 0-3 has 4 lines
    // and page 0 as 1 page.
    if(pageNumber >= pcb->pages_max || startLine >= pcb->max_lines){
        printf("Cannot load requested page #%d from file %d, which only contains %d lines!\n", pageNumber, pcb->pid, pcb->max_lines);
        return false;
    }
    // begin by placing file pointer to correct position
    char currentLine[USER_LINE_INPUT_SIZE];
    for(int i = 0; i < pcb->max_lines; i++){
        fgets(currentLine, USER_LINE_INPUT_SIZE, file);

        if(currentLine == NULL){
            break;
        } else if(i >= startLine && (i < (startLine + FRAME_SIZE))){
            // Correct lines indices to copy over.
            ram[(frameNumber * FRAME_SIZE) + offset] = strdup(currentLine);
            offset++;
        } else if(i >= (startLine + FRAME_SIZE)){
            // Line transfer done or EOF reached
            break;
        }
    }

    // Frame was not completely filled by data, so we write null in the remaining cells.
    if(offset < FRAME_SIZE){
        while(offset < FRAME_SIZE){
            ram[(frameNumber * FRAME_SIZE) + offset] = NULL;
            offset++;
        }
    }
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


