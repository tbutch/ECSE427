#include <stdio.h>
#include <stdlib.h>

// File includes
#include "../inc/shell.h"
#include "../inc/ram.h"
#include "../inc/pcb.h"
#include "../inc/interpreter.h"


int main(int argc, char ** argv){
    shellUI(argc, argv);
}

bool myInit(char* fileName){
    // Load programs into RAM
    FILE * file = fopen(fileName, "r");
    int * programStart, * programEnd;
    addToRAM(file, programStart, programEnd);

    // Create PCB for program
    PCB_t * programPCB = initPCB(*programStart, *programEnd);

    // Add PCB to ready queue
    enqueuePCB(programPCB);
    return true;
}

void scheduler(){
    bool executionDone = false;
    while(!executionDone){
        
    }
    // a. It checks to see if the CPU is available. This means that the quanta is finished or nothing is currently assigned to the CPU
    // b. It copies the PC from the PCB into the IP of the CPU
    // c. It calls the run(quanta) function within cpu.c to run the script by copying quanta lines of code from ram[] using IP into the IR, which then calls: interpreter(IR)
    // d. This executes quanta instructions from the script or until the script file is at end.
    // e. If the program is not at the end, then the PCB PC pointer is updated with the IP value and the PCB is placed at the tail of the ready queue.
    // f. If the program is at the end, then the PCB terminates (as described previously / above)
}


