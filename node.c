#include <stdio.h>
#include <stdlib.h>
#include "node.h"

/**
 * Create new node with no value
 */
node * createNode(){
    node * newNode = (node *) malloc(sizeof(node));
    newNode->next =NULL;
    return newNode;
}

/**
 * Create new node with value
 */
node * createNodeWithVal(pVoid val){
    node * newNode = (node *) malloc(sizeof(node));
    newNode->val = malloc(sizeof(val));
    newNode->val = val;
    return newNode;
}

/**
 * Dispose of character node using free
 * I assume here that the links to this node 
 * have been removed first
 */
void disposeNode(node * node){
    if(node->val != NULL){
        free(node->val);
    }
    free(node);
}

/**
 * Add node to the end of the specified list.
 */
void addToEnd(node * head, node * nextNode){
    if(head != NULL && nextNode != NULL){
        while (head->next != NULL)
        {
            head = head->next;
        }
        head->next = nextNode;
    }
}

/**
 * Remove last node from the list
 */
void removeLast(node * head){
    node * prevNode;
    node * lastNode;
    prevNode = head;
    while(prevNode->next != NULL){
        lastNode = prevNode->next;
        if(lastNode->next == NULL){
        break;
        }
        prevNode = lastNode;
    }
    prevNode->next = NULL;
    disposeNode(lastNode);
}

/**
 * Obtain length of linked list
 */ 
int getLength(node * head){
    if(head == NULL){
        return 0;
    }
    int listLength = 1;
    while(head->next != NULL){  
        listLength++;
        head=head->next;
    }
    return listLength;
}

char * convertListToString(node * head){
    int lengthOfList = getLength(head);
    char *str = malloc(lengthOfList * sizeof(char));
    int i = 0;
    if(head == NULL){
        return NULL;
    }
    str[i] = head->val;
    i++;
    while(head->next != NULL){
        str[i] = head->next->val;
        i++;
        head=head->next;
    }
    return str;
}