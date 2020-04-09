/**
 * pcb.c file. This file contains functions used to manipulate the PCB_t
 * struct, as well as the PCB ready queue, implemented as a linked list.
 * 
 * Author: Tristan Bouchard
 * Date: April 9, 2020
 * 
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// File includes
#include "../inc/pcb.h"

// Global ready list
PCB_LinkedList * readyList;

/*
 * Function: initPCB 
 * -----------------------------------------------------------------------
 *  Function used to malloc and initialize the PCB with the specified 
 *  start and end variables
 * 
 *  Returns: PCB_t pointer
 */
PCB_t * initPCB(int pid, int max_lines, int pages_max){
    PCB_t * pcb = malloc(sizeof(PCB_t));
    pcb->PC= 0;
    pcb->PC_offset = 0;
    pcb->PC_page= 0;
    pcb->pid = pid;
    pcb->max_lines=max_lines;
    pcb->pages_max=pages_max;
    for(int i = 0; i < 10; i ++){
        pcb->pageTable[i] = -1;
    }
    return pcb;
}

/*
 * Function: initPCBNode
 * -----------------------------------------------------------------------
 *  Function used to init a new PCB_Node_t
 * 
 *  Returns: PCB_Node_t pointer
 */
PCB_Node_t * initPCBNode(PCB_t * pcb){
    PCB_Node_t * pcbNode = malloc(sizeof(PCB_Node_t));
    pcbNode->pcb = pcb;
    pcbNode->next = NULL;
}

/*
 * Function: disposePCB
 * -----------------------------------------------------------------------
 *  Function used to dispose of PCB
 * 
 *  Returns: boolean on status of success
 */
bool disposePCB(PCB_t * pcb){
    if(pcb == NULL){
       return false; 
    }
    free(pcb);
    return true;
}

/*
 * Function: disposeNode
 * -----------------------------------------------------------------------
 *  Function used to dispose of PCB_Node_t as well as the PCB
 * 
 *  Returns: True on success
 */
bool disposePCBNode(PCB_Node_t *nodeToDispose) {
    if (nodeToDispose == NULL) {
        return false;
    }
    if(nodeToDispose->pcb != NULL){
        free(nodeToDispose->pcb);
    }
    free(nodeToDispose);
    return true;
}

/*
 * Function: initPCBReadyQueue
 * -----------------------------------------------------------------------
 *  Function used to initialize the PCB Ready queue
 * 
 *  Returns: true on success;
 */
PCB_LinkedList * initPCBReadyQueue() {
    readyList = malloc(sizeof(PCB_LinkedList));
    readyList->head = NULL;
    readyList->tail = NULL;
    return readyList;
}

/*
 * Function: getPCBReadyQueue
 * -----------------------------------------------------------------------
 *  Function used to obtain the PCB Ready queue pointer
 * 
 *  Returns: pointer to the ready list;
 */
PCB_LinkedList * getPCBReadyQueue(){
    return readyList;
}

/*
 * Function: clearReadyQueue
 * -----------------------------------------------------------------------
 *  Function used to clear the PCB ready queue
 * 
 *  Returns: 1 on success, 0 error;
 */
bool clearReadyQueue() {
    if (readyList == NULL) {
        return false;
    }
    PCB_Node_t *node;
    while (readyList->head != NULL) {
        node = readyList->head;
        readyList->head = readyList->head->next;
        if (!disposePCBNode(node)) {
            return false;
        }
    }
    readyList->head = NULL;
    readyList->tail = NULL;
    return true;
}

/*
 * Function: disposePCBReadyQueue
 * -----------------------------------------------------------------------
 *  Function used to dispose the PCB Ready queue. Calls clearReadyQueue
 *  before freeing the pointer
 * 
 *  Returns: true on success;
 */
bool disposePCBReadyQueue() {
    clearReadyQueue();
    free(readyList);
}

/*
 * Function: addPCBToReadyQueue
 * -----------------------------------------------------------------------
 *  Function used to add a node to the PCB ready queue.
 * 
 *  Returns: true on success;
 */
bool enqueuePCB(PCB_t * pcb) {
    if (readyList == NULL || pcb == NULL) {
        return false;
    }
    PCB_Node_t * nodeToAdd = initPCBNode(pcb);
    if(readyList->tail == NULL || readyList->head == NULL){
        readyList->head = nodeToAdd;
        readyList->tail = nodeToAdd;
        return true;
    } else {
        readyList->tail->next = nodeToAdd;
        readyList->tail = nodeToAdd;
    }
    return true;
}

/*
 * Function: dequeuePCB 
 * -----------------------------------------------------------------------
 *  Function used to remove first item from the ready queue
 * 
 *  Returns: PCB_Node_t pointer
 */
PCB_t * dequeuePCB(){
    if(readyList == NULL || readyList->head == NULL){
        return NULL;
    }
    
    PCB_Node_t * nodeToReturn = readyList->head;
    readyList->head = readyList->head->next;
    // If we have dequeued the last node in the queue, set the tail to null
    if(readyList->head == NULL){
        readyList->tail = NULL;
    }
    PCB_t * pcb_to_return = nodeToReturn->pcb;
    free(nodeToReturn);
    return pcb_to_return;
}

/*
 * Function: getPCBQueueLength 
 * -----------------------------------------------------------------------
 *  Function used to obtain the length of the PCB ready queue
 * 
 *  Returns: Length of linked list as an integer
 */
int getPCBQueueLength(){
    int length = 0;
    PCB_Node_t * node = readyList->head;
    while(node != NULL){
        length++;
        node = node->next;
    }
    return length;
}

