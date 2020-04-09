/**
 * stringUtilities.c file. This file contains a few string functions created for ease
 * of use.
 * 
 * Author: Tristan Bouchard
 * Date: April 9, 2020
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

// File includes
#include "../inc/stringUtilities.h"

/*
 * Function: isEqual
 * -----------------------------------------------------------------------
 *  Function used to compare two strings more inuitively
 * 
 *  Returns: 1 if equal, 0 if not
 */
int isEqual(char str1[], char str2[]){
    if( strcmp(str1,str2)==0 ){
        return 1;
    } else {
        return 0;
    }
}