#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"

// *************************************
// Function prototypes
void parse();
char * getInput();
void printWelcomeText();
char * getInput();
// *************************************

int main(int argc, char** argv){
    printWelcomeText();
    while(1) {
        char * inputString = getInput();
        printf("%s\n", *inputString);
        printf("$ ");
    }   
}

void parse(){
}

void printWelcomeText(){
    printf("Welcome to the Tristan Bouchard shell!\n");
    printf("Version 1.0 Created January 2020\n");
    printf("$ ");
}

/**
 * char_node * getInput()
 * 
 * Function used to obtain the input from the user without a cap on the size
 * of the input. Returns the head of the input as a linked list
 * 
 */
char * getInput(){
    char currentChar = getche();
    //printf("%c", currentChar);
    if(currentChar == '\r'){
        return NULL;
    }
    node * head = createNode();
    head->val = malloc(sizeof(currentChar));
    head->val = (char) currentChar;
    node * currentNode = head;

    while(currentChar != '\r'){
        currentChar = getche();
        //printf("%c", currentChar);
        if(currentChar != '\r'){
            node * newNode = createNode();
            head->val = malloc(sizeof(currentChar));
            head->val = (char) currentChar;
            addToEnd(head, newNode);
        }
    }
    return convertListToString(head);
}