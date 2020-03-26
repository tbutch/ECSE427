#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// File includes
#include "../inc/memorymanager.h"
#include "../inc/pcb.h"

/*
 * Function: launcher
 * -----------------------------------------------------------------------
 *  Function used to launch a program. Completes steps outlined in comments
 * 
 *  Returns: 1 if successful, 0 if not.
 */
int launcher(FILE *p){
    /*
    1. Copy the entire file into the backing store.
    2. Close the file pointer pointing to the original file.
    3. Open the file in the backing store.
    4. Our launch paging technique defaults to loading two pages of the program into RAM when it is first launched.
       A page is 4 lines of code. If the program has 4 or less lines of code, then only one page is loaded. 
       If the program has more than 8 lines of code, then only the first two pages are loaded.
    5. Modify the PCB by adding an array: int pageTable[10]; 
       The index of the array is the page number. The values stored in the cell is the frame number.
       The array is size 10 because RAM is size 10 in our simulator (4 lines of code per page).
       The PC must be the offset from the beginning of a frame, where offset is the line count starting from zero.
       Keep int PC as the pointer to the current position in the process. 
       Add int PC_page, PC_offset, pages_max. This tracks which page and offset the program is currently at,
       and the total number of pages in this program.
    */
}

int countTotalPages(FILE *f){
    /*
    Function: int countTotalPages(FILE *f) It returns the total number of pages needed by the program.
    For example if the program has 4 lines or less of code, it returns 1. 
    If greater than 4 and less that or equal to 8, it returns 2. Etc.
    */
}

void loadPage(int pageNumber, FILE *f, int frameNumber){
    /*
    FILE *f points to the beginning of the file in the backing store. The variable pageNumber is the desired page from the backing store.
    The function loads the 4 lines of code from the page into the frame in ram[].
    */
}

int findFrame(){
    /*
    Use the FIFO technique to search ram[] for a frame (not equal to NULL).
    If one exists then return its index number, otherwise return -1.
    */
}

int findVictim(PCB_t *p){
    /*
    This function is only invoke when findFrame() returns a -1. 
    Use a random number generator to pick a random frame number. If the frame number does not belong to the pages
    of the active PCB (ie. it is not in its page table) then return that frame number as the victim, 
    otherwise, starting from the randomly selected frame, iteratively increment the frame number (modulo-wise) 
    until you come to a frame number not belong to the PCB’s pages, and return that number.
    */
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

