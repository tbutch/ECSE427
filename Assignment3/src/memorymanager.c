#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <math.h>

// File includes
#include "../inc/memorymanager.h"
#include "../inc/kernel.h"
#include "../inc/interpreter.h"
#include "../inc/stringUtilities.h"
#include "../inc/shell.h"
#include "../inc/ram.h"
#include "../inc/frameQueue.h"

// Function prototypes
int countFilesInBackingStore();
FILE * copyFileToBackingStore(FILE  *p, int * pid);
int loadToRamInitial(FILE * p, PCB_t * pcb);
int countLinesInFile(FILE * f);
bool prepareFrameReadyQueue();

/*
 * Function: launcher
 * -----------------------------------------------------------------------
 *  Function used to launch a program. Completes steps outlined in comments
 * 
 *  Returns: 1 if successful, 0 if not.
 */
int launcher(FILE *p){
    // Pointer verification
    if(p == NULL){
        return 0;
    }
    //1. Copy the entire file into the backing store.
    int * pid;
    FILE * newFile = copyFileToBackingStore(p, pid);
    if(newFile == NULL){
        printf("Error opening backing store file, aborting...\n");
        return 0;
    }

    // 2. Close the file pointer pointing to the original file.
    fclose(p);
    
    // Prepare the FIFO Ready Queue by filling it with frames.
    bool readyQueuePrepared = prepareFrameReadyQueue();
    if(!readyQueuePrepared){
        printf("Frame ready queue improperly initialized, aborting...\n");
        return 0;
    }

    // 4. Our launch paging technique defaults to loading two pages of the program into RAM when it is first launched.
    //    A page is 4 lines of code. If the program has 4 or less lines of code, then only one page is loaded. 
    //    If the program has more than 8 lines of code, then only the first two pages are loaded.
    int * pageCount;
    int * lineCount;
    int totalPageCount = countTotalPages(p, pageCount, lineCount);
    if (totalPageCount < 0){
        return 0;
    }
    // Create PCB
    PCB_t * pcb = initPCB(*pid, countLinesInFile(p), totalPageCount);

    // Add initial pages to RAM
    int ramLoadStatus = loadToRamInitial(newFile, pcb);
    fclose(newFile);
    if(ramLoadStatus != SUCCESS){
        printf("Problem loading initial pages to RAM, aborting...\n");
        return 0;
    } 
}

/*
 * Function: loadToRamInitial
 * -----------------------------------------------------------------------
 *  Function used to load initial amount of pages for the program to RAM
 * 
 *  Returns: 1 if successful, 0 if not.
 */
int loadToRamInitial(FILE * p, PCB_t * pcb){
    char currentLine[USER_LINE_INPUT_SIZE];
    int totalPagesToLoad = pcb->pages_max > 2? 2: pcb->pages_max;
    for(int  pageNumber=0 ; pageNumber  < totalPagesToLoad; pageNumber ++){
        int frameNo = findFrame();
        if(frameNo == -1){
            frameNo = findVictim(pcb);
        }
        // Add required frame to RAM
        bool success = addFrameToRAM(p, pcb, pageNumber, frameNo);
        if(!success){
            return RAM_LOAD_FAIL;
        }
        // There are no victims here, so victim frame is NO_VICTIM
        updatePageTable(pcb, pageNumber, frameNo, NO_VICTIM);
    }
}

FILE * copyFileToBackingStore(FILE  *p, int * pid){
    // Get current number of files in directory
    int fileCount = countFilesInBackingStore();
    if(fileCount < 0){
        // If the directory does not exist, there was an error preparing the backing store
        return NULL;
    }

    char dir[100];
    char cmd[100];
    *pid = fileCount;
    snprintf(cmd, 100, "touch \"./BackingStore/%d\"", fileCount);
    snprintf(dir, 100, "./BackingStore/%d", fileCount);
    system(cmd);
    FILE * fp = fopen(dir, "w+");
    if(fp == NULL){
        printf("Cannot open file!\n");
        return NULL;
    }

    // Parse original file and echo that line to new file
    char buffer[USER_LINE_INPUT_SIZE];
    while(fgets(buffer, USER_LINE_INPUT_SIZE, p)){
        if(isEqual(buffer,"\n")){
            continue;
        }
        fprintf(fp, "%s", buffer);
    }
    // Reset file pointer to start
    rewind(p);
    
    return fp;
}

int countFilesInBackingStore(){
    DIR* dir = opendir(BACKING_STORE_NAME);
    struct dirent * ent;
    int count = 0;
    if (dir) {
        /* Directory exists. */
        ent = readdir(dir);
        while( ent != NULL ){
            if( !isEqual(ent->d_name, ".") && !isEqual(ent->d_name, "..") ){
                count++;
            }
            ent = readdir(dir);
        }
        closedir(dir);
    } else if (ENOENT == errno) {
        /* Directory does not exist, we have a problem lel */
        return -1;
    }
    return count;
}

/*
 * Function: countTotalPages
 * -----------------------------------------------------------------------
 *  Function used to return the total number of pages needed by the program.
 *  For example if the program has 4 lines or less of code, it returns 1. 
 *  If greater than 4 and less that or equal to 8, it returns 2. Etc.
 * 
 *  Returns: number from 1 to 10 if program of correct length, -1 otherwise
 */
int countTotalPages(FILE *f, int * pageCount, int * lineCount){
    // Reset file pointer to start
    rewind(f);
    *lineCount = countLinesInFile(f);
    double pgCount = (double) (*lineCount)/4;
    if(pgCount > 10 || pgCount <= 0){
        return -1;
    }
    *pageCount = (int) ceil(pgCount);
    // Reset file pointer to start
    rewind(f);
    return (int) pageCount;
}

/*
 * Function: countLinesInFile
 * -----------------------------------------------------------------------
 *  Function used to count total number of lines in specified file
 * 
 *  Returns: number from 1 to 10 if program of correct length, -1 otherwise
 */
int countLinesInFile(FILE * f){
    int count = 0;
    if(f == NULL){
        printf("Cannot have NULL file pointer. Aborting\n");
        return -1;
    }

    // Parse backing store file and count lines
    char buffer[USER_LINE_INPUT_SIZE];
    while(fgets(buffer, USER_LINE_INPUT_SIZE, f)){
        if(isEqual(buffer,"\n")){
            continue;
        }
        count++;
    }
    return count;
}

/*
 * Function: loadPage
 * -----------------------------------------------------------------------
 *  FILE *f points to the beginning of the file in the backing store. The variable pageNumber (0 <= x <= 9) 
 *  is the desired page from the backing store. The function loads the 4 lines of code from the page into the frame in ram[].
 * 
 *  Returns: void
 */
void loadPage(int pageNumber, FILE *f, int frameNumber){
    rewind(f);


}

/*
 * Function: findFrame
 * -----------------------------------------------------------------------
 *  Use the FIFO technique to search ram[] for a frame (not equal to NULL).
 *  If one exists then return its index number, otherwise return -1.
 * 
 *  Returns: void
 */
int findFrame(){
    // Make use of the ready queue.
    return dequeueFrame();
}

int findVictim(PCB_t *p){
    /*
    This function is only invoke when findFrame() returns a -1. 
    Use a random number generator to pick a random frame number. If the frame number does not belong to the pages
    of the active PCB (ie. it is not in its page table) then return that frame number as the victim, 
    otherwise, starting from the randomly selected frame, iteratively increment the frame number (modulo-wise) 
    until you come to a frame number not belong to the PCB’s pages, and return that number.
    */
   return -1;
}

int updatePageTable(PCB_t *p, int pageNumber, int frameNumber, int victimFrame){
    /*
    The page tables must also be updated to reflect the changes. 
    If a victim was selected then the PCB page table of the victim must be updated.
    We do this once for the PCB asking for the page fault, and we might do it again for the victim PCB 
    (if there was one).
    p->pageTable[pageNumber] = frameNumber (or = victimFrame).
    */
}

/*
 * Function: prepareFrameReadyQueue
 * -----------------------------------------------------------------------
 *  At program start, add all frames to ready queue.
 * 
 *  Returns: boolean on success status
 */
bool prepareFrameReadyQueue(){
    for(int i = 0; i < NUMBER_OF_FRAMES; i++){
        bool success = enqueueFrame(i);
        if(!success){
            return false;
        }
    }
    return true;
}
