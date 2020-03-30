#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/cpu.h"
#include "../inc/ram.h"
#include "../inc/interpreter.h"

/*
 * Function: initCPU
 * -----------------------------------------------------------------------
 *  Function used to initialize the CPU struct. Initializes the quanta to 2,
 *  Instruction Pointer (IP) to 0 and Instruction Register (IR) to a null string.
 * 
 *  Returns: CPU_t pointer
 */
CPU_t * initCPU(){
    cpu = malloc(sizeof(CPU_t));
    cpu->quanta = BASE_QUANTA;
    cpu->offset = 0;
    cpu->IP = CPU_START_ADDRESS;
    cpu->IR[0] = '\0';
    return cpu;
}

/*
 * Function: disposeCPU
 * -----------------------------------------------------------------------
 *  Function used to free CPU defined in the header file.
 * 
 *  Returns: void
 */
void disposeCPU(){
    free(cpu);
}

/*
 * Function: run
 * -----------------------------------------------------------------------
 *  Function used to run programs for a given quanta.
 * 
 *  Returns: new instruction offset of CPU
 */
int run(int quanta, mem_t * shellMemory[], int shellMemoryMaxSize, int maxInputSize){
    cpu->quanta=quanta;
    while(cpu->quanta > 0){
    //for(int i = 0; i < quanta; i++){
        // Verify if instruction in RAM is null --> This would indicate end of program
        if(ram[cpu->IP + cpu->offset] == NULL){
            // Script execution is completed
            break;
        }
        // Fetch instruction from ram to CPU IR
        strcpy(cpu->IR, ram[cpu->IP + cpu->offset]);
        // call interpreter.
        int status = parseAndEvaluate(cpu->IR, shellMemory, shellMemoryMaxSize, maxInputSize);
        // Increment Instruction pointer
        cpu->quanta--;
        cpu->offset++;
    }
    cpu->quanta=BASE_QUANTA;
    return cpu->offset;
}