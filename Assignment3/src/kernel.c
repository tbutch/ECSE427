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

    launcher(file);

    // int * programStart = malloc(sizeof(int));
    // int * programEnd = malloc(sizeof(int));
    // *programStart = -1;
    // *programEnd = -1;
    // if(!addToRAM(file, programStart, programEnd)){
    //     return RAM_LOAD_FAIL;
    // }
    // fclose(file);
    // // Create PCB for program
    // PCB_t * programPCB = initPCB(*programStart, *programEnd);
    // free(programStart);
    // free(programEnd);
    // // Add PCB to ready queue
    // enqueuePCB(programPCB);
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
    bool executionDone = false;
    
    // Initialize the CPU
    while(!executionDone){
        // a. It checks to see if the CPU is available. This means that the quanta is finished or nothing is currently assigned to the CPU
        if(cpu->quanta == BASE_QUANTA){
            // b. It copies the PC from the PCB into the IP of the CPU
            PCB_t * pcb = dequeuePCB();

            // If the PCB returned is NULL, then list is empty!
            if(pcb == NULL){
                executionDone = true;
                break;
            }
            cpu->IP = pcb->PC;
            // c. It calls the run(quanta) function within cpu.c to run the script by copying quanta lines of code from ram[] using IP into the IR, which then calls: interpreter(IR)

            // TODO: FIX THIS SHIT
            int quantaToRun = (pcb->max_lines - pcb->PC + 1 > BASE_QUANTA) ? BASE_QUANTA : pcb->max_lines - pcb->PC + 1;
            int newPC = run(quantaToRun, shellMemory, shellMemoryMaxSize, maxInputSize);
            pcb->PC = newPC;
            if(pcb->PC > pcb->max_lines){
                // f. If the program is at the end, then the PCB terminates (as described previously / above)
                disposePCB(pcb);
            } else {
                // e. If the program is not at the end, then the PCB PC pointer is updated with the IP value and the PCB is placed at the tail of the ready queue.
                enqueuePCB(pcb);
            }
        }
    }
    cleanRam();

    return SUCCESS;
}


