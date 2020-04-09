/**
 * frameQueue.c file. This file contains the functions used to manipulate
 * the frameReadyList linked list, which contains available RAM frames.
 * 
 * Author: Tristan Bouchard
 * Date: April 9, 2020
 * 
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../inc/frameQueue.h"

// Global ready list
Frame_LinkedList * frameReadyList;

/*
 * Function: initFrameNode
 * -----------------------------------------------------------------------
 *  Function used to init a new Frame_Node_t
 * 
 *  Returns: Frame_Node_t pointer
 */
Frame_Node_t * initFrameNode(int frameNo){
    Frame_Node_t * frameNode = malloc(sizeof(Frame_Node_t));
    frameNode->frameNo=frameNo;
    frameNode->next = NULL;
    return frameNode;
}

/*
 * Function: disposeNode
 * -----------------------------------------------------------------------
 *  Function used to dispose of Frame_Node_t
 * 
 *  Returns: True on success
 */
bool disposeFrameNode(Frame_Node_t *nodeToDispose) {
    if (nodeToDispose == NULL) {
        return false;
    }
    free(nodeToDispose);
    return true;
}

/*
 * Function: initFrameReadyQueue
 * -----------------------------------------------------------------------
 *  Function used to initialize the Frame Ready queue
 * 
 *  Returns: true on success;
 */
Frame_LinkedList * initFrameReadyQueue() {
    frameReadyList = malloc(sizeof(Frame_LinkedList));
    frameReadyList->head = NULL;
    frameReadyList->tail = NULL;
    return frameReadyList;
}

/*
 * Function: getFrameReadyQueue
 * -----------------------------------------------------------------------
 *  Function used to obtain the Frame Ready queue pointer
 * 
 *  Returns: pointer to the ready list;
 */
Frame_LinkedList * getFrameReadyQueue(){
    return frameReadyList;
}

/*
 * Function: clearFrameQueue
 * -----------------------------------------------------------------------
 *  Function used to clear the Frame ready queue
 * 
 *  Returns: 1 on success, 0 error;
 */
bool clearFrameQueue() {
    if (frameReadyList == NULL) {
        return false;
    }
    Frame_Node_t *node;
    while (frameReadyList->head != NULL) {
        node = frameReadyList->head;
        frameReadyList->head = frameReadyList->head->next;
        if (!disposeFrameNode(node)) {
            return false;
        }
    }
    frameReadyList->head = NULL;
    frameReadyList->tail = NULL;
    return true;
}

/*
 * Function: disposeFrameReadyQueue
 * -----------------------------------------------------------------------
 *  Function used to dispose the Frame Ready queue. Calls clearReadyQueue
 *  before freeing the pointer
 * 
 *  Returns: true on success;
 */
bool disposeFrameReadyQueue() {
    clearFrameQueue();
    free(frameReadyList);
}

/*
 * Function: enqueueFrame
 * -----------------------------------------------------------------------
 *  Function used to add a node to the frame ready queue.
 * 
 *  Returns: true on success;
 */
bool enqueueFrame(int frameNo) {
    if (frameReadyList == NULL) {
        return false;
    }
    Frame_Node_t * nodeToAdd = initFrameNode(frameNo);
    if(frameReadyList->tail == NULL || frameReadyList->head == NULL){
        frameReadyList->head = nodeToAdd;
        frameReadyList->head = nodeToAdd;
        frameReadyList->tail = nodeToAdd;
        return true;
    } else {
        frameReadyList->tail->next = nodeToAdd;
        frameReadyList->tail = nodeToAdd;
    }
    return true;
}

/*
 * Function: dequeueFrame 
 * -----------------------------------------------------------------------
 *  Function used to remove first item from the ready queue
 * 
 *  Returns: int frame number
 */
int dequeueFrame(){
    if(frameReadyList == NULL || frameReadyList->head == NULL){
        return -1;
    }
    
    Frame_Node_t * nodeToReturn = frameReadyList->head;
    frameReadyList->head = frameReadyList->head->next;
    // If we have dequeued the last node in the queue, set the tail to null
    if(frameReadyList->head == NULL){
        frameReadyList->tail = NULL;
    }
    int frameNo_to_return = nodeToReturn->frameNo;
    free(nodeToReturn);
    return frameNo_to_return;
}

/*
 * Function: getFrameQueueLength 
 * -----------------------------------------------------------------------
 *  Function used to obtain the length of the Frame ready queue
 * 
 *  Returns: Length of linked list as an integer
 */
int getFrameQueueLength(){
    int length = 0;
    Frame_Node_t * node = frameReadyList->head;
    while(node != NULL){
        length++;
        node = node->next;
    }
    return length;
}