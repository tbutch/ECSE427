#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/node.h"
#include "../inc/shell.h"

// *************************************
// Function prototypes
void parse();
char * getInput();
void printWelcomeText();
char * getInput();
// *************************************

// Global Variables
char prompt[100] = {'$', ' '};
char userInput[1000];
int errorCode = 0;
//


int main(int argc, char** argv){
    printWelcomeText();
    while(1) {
        printf("%s", prompt);
        fgets(userInput, INPUT_SIZE -1, stdin);
        parse();
    }   
}

void parse(){
}

void printWelcomeText(){
    printf("Welcome to the Tristan Bouchard shell!\n");
    printf("Version 1.0 Created January 2020\n");
}
