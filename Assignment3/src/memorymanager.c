/**
 * memorymanager.c file. This file contains the main memory managing functions such as 
 * loading program pages to RAM, initiating RAM slots, taking care of updating page tables
 * and maintaining the backing store.
 * 
 * Author: Tristan Bouchard
 * Date: April 9, 2020
 * 
 */

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
int loadToRamInitial(PCB_t * pcb);
int countLinesInFile(FILE * f);
bool prepareFrameReadyQueue();
PCB_t * findVictimPCB(int frameNo);
void freeUsedRAM(PCB_t * pcb);

/*
 * Function: launcher
 * -----------------------------------------------------------------------
 *  Function used to launch a program. Completes steps outlined in comments
 * 
 *  Returns: 1 if successful, 0 if not.
 */
int launcher(FILE *p){
    // File pointer verification
    if(p == NULL){
        return 0;
    }
    //1. Copy the entire file into the backing store.
    int * pid = malloc(sizeof(int));
    FILE * newFile = copyFileToBackingStore(p, pid);
    
    // 2. Close the file pointer pointing to the original file.
    fclose(p);

    if(newFile == NULL){
        printf("Error opening backing store file, aborting...\n");
        return 0;
    }

    int * pageCount = malloc(sizeof(int));
    int * lineCount = malloc(sizeof(int));
    countTotalPages(newFile, pageCount, lineCount);
    if (*pageCount < 0){
        // Program of improper length.
        printf("Program does not contain the correct number of pages.\n");
        printf("Pages counted: %d.\n", *pageCount);
        return 0;
    }
    // Create PCB
    PCB_t * pcb = initPCB(*pid, *lineCount, *pageCount);

    free(pid);
    free(pageCount);
    free(lineCount);
    fclose(newFile);
    // Add initial pages to RAM - This function also updates the PCB page table
    int ramLoadStatus = loadToRamInitial(pcb);
    if(ramLoadStatus != SUCCESS){
        printf("Problem loading initial pages to RAM, aborting...\n");
        return 0;
    }

    // Add PCB to ready list.
    bool enqueueSuccess = enqueuePCB(pcb);
    if(!enqueueSuccess){
        printf("Error adding PCB to ready queue.\n");
        printf("Please verify that ready queue is properly initialized.\n");
        return 0;
    }
    return 1;
}

/*
 * Function: loadToRamInitial
 * -----------------------------------------------------------------------
 *  Function used to load initial amount of pages for the program to RAM
 * 
 *  Our launch paging technique defaults to loading two pages of the program into RAM when it is first launched.
 *  A page is 4 lines of code. If the program has 4 or less lines of code, then only one page is loaded. 
 *  If the program has more than 8 lines of code, then only the first two pages are loaded.
 * 
 *  Returns: Success if successful, RAM_LOAD_FAIL if not.
 */
int loadToRamInitial(PCB_t * pcb){
    char currentLine[USER_LINE_INPUT_SIZE];
    int totalPagesToLoad = pcb->pages_max > 2? 2: pcb->pages_max;
    for(int pageNumber=0 ; pageNumber  < totalPagesToLoad; pageNumber++){
        loadPage(pageNumber, pcb);
        if(pageNumber == 0){
            // PC is initiated to wherever the first page is loaded in RAM
            pcb->PC = (pcb->pageTable[0] * FRAME_SIZE);
            pcb->PC_page = 0;
            pcb->PC_offset = 0;
        }
    }
    return SUCCESS;
}

/*
 * Function: copyFileToBackingStore
 * -----------------------------------------------------------------------
 *  Function used to copy specified file to baking store. Given
 *  file must be less than RAMSIZE number of lines in length.
 * 
 *  Returns: Backing store file pointer.
 */
FILE * copyFileToBackingStore(FILE  *p, int * pid){
    // Get current number of files in directory
    int fileCount = countFilesInBackingStore();
    if(fileCount < 0){
        // If the directory does not exist, there was an error preparing the backing store
        return NULL;
    }
    // Verify that file contains less than RAMSIZE lines.
    rewind(p);
    int lineCount = countLinesInFile(p);
    rewind(p);

    if(! (lineCount <= RAMSIZE)){
        printf("Specified file contains %d lines. The maximum amount of lines is %d\n", lineCount, RAMSIZE);
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

    // Flush file stream to write to it without closing the pointer
    fflush(fp);
    return fp;
}

/*
 * Function: countFilesInBackingStore
 * -----------------------------------------------------------------------
 *  Function used to count the files currently in the backing store. This
 *  is principally used to obtain a process ID number during program initiation
 * 
 *  Returns: Backing store file pointer.
 */
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
    double pgCount = (double) (*lineCount)/FRAME_SIZE;
    if(pgCount > NUMBER_OF_FRAMES || pgCount <= 0){
        return -1;
    }
    *pageCount = (int) ceil(pgCount);
    // Reset file pointer to start
    rewind(f);
    return (int) *pageCount;
}

/*
 * Function: countLinesInFile
 * -----------------------------------------------------------------------
 *  Function used to count total number of lines in specified file. This function
 *  ignores newlines, as when copied to the backing store, newlines are not copied.
 * 
 *  Returns: number from 1 to 10 if program of correct length, -1 otherwise
 */
int countLinesInFile(FILE * f){
    int count = 0;
    if(f == NULL){
        printf("Cannot have NULL file pointer. Aborting\n");
        return -1;
    }

    // Parse file and count lines
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
 *  Request specified page to be loaded to RAM from this PCB.
 *  The variable pageNumber (0 <= x <= 9) is the desired page from the backing store. 
 *  The function loads the 4 lines of code from the page into the frame in ram[] and returns
 *  the victim frame, if no frames were available.
 * 
 *  Returns: frameNumber to which it was loaded.
 */
bool loadPage(int pageNumber, PCB_t * pcb){

    // Open file
    char dir[100];
    snprintf(dir, 100, "./BackingStore/%d", pcb->pid);
    FILE * p = fopen(dir, "r+");

    if(p == NULL){
        printf("Error opening file %d from backing store\n. Aborting", pcb->pid);
    }
    
    bool victim = false;
    int frameNo = findFrame();
    if(frameNo == -1){
        victim = true;
        frameNo = findVictim(pcb);
    }
    // Add required frame to RAM
    bool success = addPageToRAM(p, pcb, pageNumber, frameNo);
    if(!success){
        return RAM_LOAD_FAIL;
    }
    fclose(p);
    // Update victim PCB to tell it that its program is no longer in RAM
    if(victim){
        PCB_t * victimPCB = findVictimPCB(frameNo);
        if(victimPCB == NULL){
            // Problem with findVictim();
            return false;
        }
        updatePageTable(victimPCB, pageNumber, frameNo, frameNo);
        int w = 0;
    }
    // Update pcb of our file.
    updatePageTable(pcb, pageNumber, frameNo, NO_VICTIM);
    return true;

}

/*
 * Function: isPageLoaded
 * -----------------------------------------------------------------------
 *  Verify the PCB to determine if the page is loaded to RAM
 * 
 *  Returns: true if loaded, false otherwise
 */
bool isPageLoaded(int pageNumber, PCB_t * pcb){
    return pcb->pageTable[pageNumber] == -1 ? false : true;
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

/*
 * Function: findVictimPCB
 * -----------------------------------------------------------------------
 *  This function is used to obtain the victim PCB that contains the frame 
 *  specified by findVictim. It searches the PCB's in the ready queue, which
 *  there must be at least one, or else the findVictim function is broken.
 * 
 *  Returns: void
 */
PCB_t * findVictimPCB(int frameNo){
    PCB_LinkedList * list = getPCBReadyQueue();
    PCB_Node_t * head = list->head;
    if(head == NULL){
        // UH OH, how did we get here? We shoudln't be calling
        // this function without knowing that there is a program
        // being victimized! If there are no PCB's in the queue, then WTF?!?
        printf("UH OH. Issue finding RAM victim. Aborting\n");
        return NULL;
    }
    while(head!=NULL){
        for(int i = 0; i < NUMBER_OF_FRAMES; i++){
            if(head->pcb->pageTable[i] == frameNo){
                return head->pcb;
            } 
        }
        head=head->next;
    }
    // If we got here without finding a victim, we have a problem.
    return NULL;
}

/*
 * Function: findVictim
 * -----------------------------------------------------------------------
 *  This function is only invoke when findFrame() returns a -1.
 *  We use a RNG to get a random number from 0 to NUMBER_OF_FRAMES -1. We then
 *  check to see if that frame belongs.
 * 
 *  Returns: void
 */
int findVictim(PCB_t *pcb){
   int randomNum = rand() % NUMBER_OF_FRAMES;
   if(randomNum < 0 || randomNum > NUMBER_OF_FRAMES -1){
       printf("Error generating random frame number. Generated number was:\n");
       printf("\t%d\n. It should be within the range [0, %d]", randomNum, NUMBER_OF_FRAMES-1);
   }
   while(true){
       for(int i = 0; i < NUMBER_OF_FRAMES; i++){
           if(pcb->pageTable[i] == randomNum){
                randomNum = (randomNum + 1) % NUMBER_OF_FRAMES;
                // Set to -1 so that counter is reset to 0
                i = -1;
           }
       }
       break;
   }
   return randomNum;
}

/*
 * Function: updatePageTable
 * -----------------------------------------------------------------------
 *  Update page table for specified PCB. If there is no victim frame, it is 
 *  set to -1 to indicate there is no need to update the other PCB.
 */
int updatePageTable(PCB_t * pcb, int pageNumber, int frameNumber, int victimFrame){
    if(victimFrame == -1){
        pcb->pageTable[pageNumber] = frameNumber;
    } else {
        // We need to set the page in the victim pcb to -1 where the related frame is found
        // This needs to be done because it is not necessarily true that a certain page for
        // a PCB equates to that same page for another PCB
        for(int i = 0; i < NUMBER_OF_FRAMES; i++){
            if(pcb->pageTable[i] == victimFrame){
                pcb->pageTable[i] = -1;
                break;
            }
        }
    }
}

/*
 * Function: deleteFileInBackingStore
 * -----------------------------------------------------------------------
 *  Function used to delete file in Backing Store one execution is complete.
 * 
 * returns: Boolean on success.
 */
bool deleteFileInBackingStore(PCB_t * pcb){
    char cmd[100];
    snprintf(cmd, 100, "del /f /q \".\\\\BackingStore\\\\%d\"", pcb->pid);
    freeUsedRAM(pcb);
    return true;
}

/*
 * Function: freeUsedRAM
 * -----------------------------------------------------------------------
 *  Function used to free the RAM a program was using and allow those frames
 *  to be used once again by adding them to the frame ready queue.
 * 
 * returns: void
 */
void freeUsedRAM(PCB_t * pcb){
    for(int i = 0; i < NUMBER_OF_FRAMES; i++){
        if(pcb->pageTable[i] != -1){
            enqueueFrame(i);
        }
    }
}