/**
 * frameQueue header file. Here, the frameQueue Linked list struct is defined and the available
 * API is described.
 * 
 * Author: Tristan Bouchard
 * Date: April 29, 2020
 * 
 */

#ifndef FRAME_QUEUE
#define FRAME_QUEUE

// Define PCB node
typedef struct Frame_Node_t{
    int frameNo;
    struct Frame_Node_t * next;
} Frame_Node_t;

// Define PCB linked list
typedef struct{
    Frame_Node_t * head;
    Frame_Node_t * tail;
} Frame_LinkedList;

// Public function prototypes
Frame_Node_t * initFrameNode(int frameNo);
bool disposeFrameNode(Frame_Node_t * frame);
Frame_LinkedList * initFrameReadyQueue();
Frame_LinkedList * getFrameReadyQueue();
bool clearFrameQueue();
bool disposeFrameQueue();
bool enqueueFrame(int frameNo);
int dequeueFrame();
int getFrameQueueLength();

#endif