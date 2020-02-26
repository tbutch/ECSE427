#ifndef PCB
#define PCB

typedef struct{
    int PC;
    int start;
    int end;
} PCB_t;

// Define PCB node
typedef struct{
    PCB_t * pcb;
    PCB_t * next;
} PCB_Node_t;

// Define PCB linked list
typedef struct{
    PCB_Node_t * head;
    PCB_Node_t * tail;
} PCB_LinkedList;

bool enqueuePCB(PCB_LinkedList * list, PCB_Node_t * node);
bool clearReadyQueue(PCB_LinkedList * list);

#endif
