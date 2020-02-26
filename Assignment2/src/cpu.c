#include <stdio.h>
#include <stdlib.h>

#include "../inc/cpu.h"

/*
 * Function: initCPU
 * -----------------------------------------------------------------------
 *  Function used to initialize the CPU struct. Initializes the quanta to 2,
 *  Instruction Pointer (IP) to 0 and Instruction Register (IR) to a null string.
 * 
 *  Returns: 1 on success, 0 error;
 */
int initCPU(CPU_t * cpu){
    cpu->quanta = 2;
    cpu->IP = 0;
    cpu->IR[0] = '\0';
    return 1;
}