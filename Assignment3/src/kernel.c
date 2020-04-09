#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

// File includes
#include "../inc/kernel.h"
#include "../inc/shell.h"
#include "../inc/ram.h"
#include "../inc/pcb.h"
#include "../inc/interpreter.h"
#include "../inc/cpu.h"
#include "../inc/stringUtilities.h"
#include "../inc/memorymanager.h"
#include "../inc/frameQueue.h"

// Function prototypes
int boot();
int prepareBackingStore();
void updatePCB_PC_offset(PCB_t * pcb, int offset);
int pageFault(PCB_t * pcb);

/*
 * Function: main
 * -----------------------------------------------------------------------
 *  Function used as main entry point of the program.
 * 
 *  Returns: 0 if successful, 1 if not.
 */
int main(int argc, char ** argv){
    int bootStatus = boot();
    if(bootStatus != SUCCESS){
        printf("Failure detected in kernel boot sequence.\n");
        return 1;
    }
    return shellUI(argc, argv);
}

/*
 * Function: boot
 * -----------------------------------------------------------------------
 *  Function used to prepare shell environment.
 * 
 *  Returns: 0 if successful, 1 if not.
 */
int boot(){
    // Init PCB list, RAM, CPU
    printf("----Initiate Boot Sequence----\n\n");
    printf("Preparing Ready Queue...");
    initPCBReadyQueue();
    printf(" Done.\n");
    printf("Preparing RAM...");
    initRam();
    printf(" Done.\n");
    printf("Preparing FIFO Frame Queue...");
    initFrameReadyQueue();
    printf(" Done.\n");
    printf("Preparing CPU...");
    initCPU();
    printf(" Done.\n");
    printf("Preparing Backing Store...");
    int status = prepareBackingStore();
    printf(" Done.\n");
    printf("\n----Boot Sequence Completed!----\n\n");
    return status;
}

/*
 * Function: prepareBackingStore
 * -----------------------------------------------------------------------
 *  Function used to prepare exec backing store. It makes sure there is nothing in the Backing 
 *  Store on kernel startup
 * 
 *  Returns: Error code if not successful, as defined in interpreter.h.
 */
int prepareBackingStore(){
    system("rmdir /Q /S \"./BackingStore\"");
    system("mkdir \"./BackingStore\"");
    return SUCCESS;
}

/*
 * Function: myInit
 * -----------------------------------------------------------------------
 *  Function used to initialize exec'd programs as PCB's in the ready queue.
 * 
 *  Returns: 0 if successful, 1 if not.
 */
int myInit(char* fileName){
    // Load programs into RAM
    FILE * file = fopen(fileName, "r");
    if(file == NULL){
        return NONEXISTANT_FILE;
    }

    int status = launcher(file);
    if(status == 0){
        return FAILURE;
    }
    return SUCCESS;
}

/*
 * Function: scheduler
 * -----------------------------------------------------------------------
 *  Function used to run programs loaded in ram untill execution complete
 * 
 *  Returns: Error codes as specified in interpreter.h
 */
int scheduler(mem_t * shellMemory[], int shellMemoryMaxSize, int maxInputSize){
    // Re-init CPU before scheduler runs, just in case
    resetCPU();
    // Initialize the CPU
    while(true){
        // a. It checks to see if the CPU is available. This means that the quanta is finished or nothing is currently assigned to the CPU
        if(cpu->quanta == BASE_QUANTA){
            // b. It copies the PC from the PCB into the IP of the CPU
            PCB_t * pcb = dequeuePCB();
            // If the PCB returned is NULL, then list is empty!
            if(pcb == NULL){
                break;
            }
            // Determine if required page is loaded in RAM
            bool pageLoaded = isPageLoaded(pcb->PC_page, pcb);
            if(!pageLoaded){
                // page fault sequence.
                int status = pageFault(pcb);
                if(status != SUCCESS){
                    return status;
                }
                enqueuePCB(pcb);
                continue;
            }

            cpu->IP = pcb->PC;
            cpu->offset = pcb->PC_offset;
            // c. It calls the run(quanta) function within cpu.c to run the script by copying quanta lines of code from ram[] using IP into the IR, which then calls: interpreter(IR)

            // TODO: Verify THIS SHIT
            //int quantaToRun = (pcb->max_lines - (((pcb->PC_page + 1) * FRAME_SIZE) - 1) > BASE_QUANTA) ? BASE_QUANTA : pcb->max_lines - pcb->PC + 1;
            int quantaToRun = BASE_QUANTA;
            int newOffset = run(quantaToRun, shellMemory, shellMemoryMaxSize, maxInputSize);

            // Update PC and offset
            updatePCB_PC_offset(pcb, newOffset);
            
            // Verify if at page end
            if(((pcb->PC_page * FRAME_SIZE) + pcb->PC_offset) >= pcb->max_lines){
                // f. If the program is at the end, then the PCB terminates (as described previously / above)
                deleteFileInBackingStore(pcb);
                // Free ram and add free frames back to queue
                disposePCB(pcb);
                
            } else {
                // e. If the program is not at the end, then the PCB PC pointer is updated with the IP value and the PCB is placed at the tail of the ready queue.
                pageLoaded = isPageLoaded(pcb->PC_page, pcb);
                if(!pageLoaded){
                    // page fault sequence.
                    int status = pageFault(pcb);
                    if(status != SUCCESS){
                        return status;
                    }
                    pcb->PC = pcb->pageTable[pcb->PC_page] * FRAME_SIZE;
                }
                enqueuePCB(pcb);
            }
        }
    }
    cleanRam();
    clearFrameQueue();
    prepareBackingStore();
    return SUCCESS;
}


/*
 * Function: updatePCB_PC_offset
 * -----------------------------------------------------------------------
 *   Function used to update the PCB and PC offset after execution returns.
 * 
 *  Returns: 0 if successful, 1 if not.
 */
void updatePCB_PC_offset(PCB_t * pcb, int newOffset){
    if(newOffset == FRAME_SIZE){
        // We have reached page end!
        //pcb->PC = pcb->PC + FRAME_SIZE;
        pcb->PC_page++;
        pcb->PC = pcb->pageTable[pcb->PC_page] * FRAME_SIZE;
        pcb->PC_offset = 0;
    } else {
        pcb->PC_offset = newOffset;
    }
}

/*
 * Function: pageFault
 * -----------------------------------------------------------------------
 *   Function used when a page needed by the program is not loaded in RAM
 * 
 *  Returns: Error codes as specified in shell.h
 */
int pageFault(PCB_t * pcb){
    // PageFault sequence
    loadPage(pcb->PC_page, pcb);
    // TODO: Update PC! How because I don't have the frame number? --> Make load page return new frame
    // Check the PCB pagetable and find the frame that way!
    // Update PC
    int frameNo = pcb->pageTable[pcb->PC_page];
    if(frameNo == -1){
        return RAM_LOAD_FAIL;
    }
    return SUCCESS;
}