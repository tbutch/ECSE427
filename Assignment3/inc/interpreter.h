/**
 * Interpreter header file. Contains all of the public functions available
 * to includers of the file, as well as helpful constant strings for 
 * matching commands.
 * 
 * Author: Tristan Bouchard
 * Date: January 29, 2020
 * 
 */

#ifndef INTERPRETER
#define INTERPRETER

#include "../inc/shellmemory.h"

// Constant string definitions for command matching
static const char HELP_COMMAND[] ="help";
static const char QUIT_COMMAND[] ="quit";
static const char SET_COMMAND[] ="set";
static const char PRINT_COMMAND[] ="print";
static const char RUN_COMMAND[] ="run";
static const char EXEC_COMMAND[] ="exec";

// Add exit command cus Its exit in every other shell lol
static const char EXIT_COMMAND[] ="exit";

// Exit codes - Makes it clearer when returning an
// code from a function 
static const int FATAL_ERROR = -1;
static const int FAILURE = 0;
static const int SUCCESS = 1;
static const int UNKOWN_COMMAND = 2;
static const int MALFORMED_COMMAND = 3;
static const int QUIT_TERMINAL = 4;
static const int NONEXISTANT_FILE = 5;
static const int RAM_LOAD_FAIL = 6;

// Public functions
int parseAndEvaluate(char input[], mem_t * shellMemory[], int shellMemoryMaxSize, int maxInputSize);

#endif