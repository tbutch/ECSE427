/**
 * Main shell definition file. Preprocessor definitions for constant
 * integers, such as the shell size and the user input line size can 
 * all be found and modified here, if necessary
 * 
 * Author: Tristan Bouchard
 * Date: January 29, 2020
 * 
 */

#ifndef SHELL
#define SHELL

// Maximum line size from input
#define USER_LINE_INPUT_SIZE 1000

// Maximum shell memory size. This variable defines how many
// variables can be stored in the shell memory.
#define SHELL_MEMORY_SIZE 100

// Main shell UI function
int shellUI(int argc, char** argv);

#endif