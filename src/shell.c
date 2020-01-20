#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/node.h"
#include "../inc/shell.h"
#include "../inc/interpreter.h"

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
        parse(userInput);
    }   
}

/**
 * Function used to parse user input. Uses the user input
 * array and tokenizes it into words in a 2D array, which
 * is passed to the interpreter.
 * 
 */
void parse(char input[]){
    int i = 0;
    int wordCount = 0;
    int currWordLength = 0;
    char currWord[100];
    char * wordArray[100]; // Word array, of length 100

    // Parse input string while not null
    while(input[i] != '\0'){
        char currentChar = input[i];

        // If the currentChar is not a space, add it to the 
        // current string
        
        if(currentChar == ' '){
            // If it is a space, add a null to the end of the current
            // word and add it to the word array
            currWord[currWordLength] = '\0';
            wordArray[wordCount] = strdup(currWord);
            strcpy(currWord,"");
            currWordLength = 0;
            wordCount++;
        } 
        // if end of string
        else if(currentChar == '\n'){
            currWord[currWordLength] = '\0';
            wordArray[wordCount] = strdup(currWord);
            strcpy(currWord,"");
            wordCount++;
            break;
        }
        else {
            currWord[currWordLength] = currentChar;
            currWordLength++;
        }
        i++;
    }
    strcpy(currWord,"\0");
    wordArray[wordCount] = strdup(currWord); 
    parseInput(wordArray);
}

void printWelcomeText(){
    printf("Welcome to the Tristan Bouchard shell!\n");
    printf("Version 1.0 Created January 2020\n");
}