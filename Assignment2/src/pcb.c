#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "..\inc\pcb.h"

// Private function prototypes
bool disposeNode(PCB_Node_t *nodeToDispose);

/*
 * Function: addPCBToReadyQueue
 * -----------------------------------------------------------------------
 *  Function used to add a node to the PCB ready queue.
 * 
 *  Returns: true on success;
 */
bool enqueuePCB(PCB_LinkedList *list, PCB_Node_t *node) {
    if (list == NULL || node == NULL) {
        return false;
    }
    PCB_Node_t *currNode = list->head;
    while (currNode != NULL) {
        currNode = currNode->next;
    }
    currNode = node;
    list->tail = node;
    return true;
}

/*
 * Function: clearReadyQueue
 * -----------------------------------------------------------------------
 *  Function used to clear the PCB ready queue
 * 
 *  Returns: 1 on success, 0 error;
 */
bool clearReadyQueue(PCB_LinkedList *list) {
    if (list == NULL) {
        return false;
    }
    PCB_Node_t *node;
    while (list->head != NULL) {
        node = list->head;
        list->head = list->head->next;
        if (!disposeNode(node)) {
            return false;
        }
    }
    return true;
}

/*
 * Function: initPCBReadyQueue
 * -----------------------------------------------------------------------
 *  Function used to initialize the PCB Ready queue
 * 
 *  Returns: true on success;
 */
bool initPCBReadyQueue(PCB_LinkedList *list) {
    if (list == NULL) {
        return false;
    }
    list->head = NULL;
    list->tail = NULL;
    return true;
}

/*
 * Function: disposeNode
 * -----------------------------------------------------------------------
 *  Function used to dispose of PCB_Node_t
 * 
 *  Returns: True on success
 */
bool disposeNode(PCB_Node_t *nodeToDispose) {
    if (nodeToDispose == NULL) {
        return false;
    }
    free(nodeToDispose->pcb);
    free(nodeToDispose);
    return true;
}

/*
 * Function: dequeuePCB 
 * -----------------------------------------------------------------------
 *  Function used to remove first item from the ready queue
 * 
 *  Returns: void
 */
PCB_Node_t * dequeuePCB(PCB_LinkedList * list){
    PCB_Node_t * nodeToReturn = list->head;
    list->head=list->head->next;
}