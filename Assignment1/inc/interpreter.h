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
static const char helpCommand[] ="help";
static const char quitCommand[] ="quit";
static const char setCommand[] ="set";
static const char printCommand[] ="print";
static const char runCommand[] ="run";

// Exit codes - Makes it clearer when returning an
// code from a function 
static const int FATAL_ERROR = -1;
static const int FAILURE = 0;
static const int SUCCESS = 1;
static const int UNKOWN_COMMAND = 2;
static const int MALFORMED_COMMAND = 3;
static const int QUIT_TERMINAL = 4;

// Public functions
int parseAndEvaluate(char input[], mem_t * shellMemory[], int shellMemoryMaxSize, int maxInputSize);
int isEqual(char str1[], char str2[]);

#endif