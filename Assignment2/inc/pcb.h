#ifndef PCB
#define PCB

typedef struct{
    int PC;
    int start;
    int end;
} PCB_t;

// Define PCB node
typedef struct PCB_Node_t{
    PCB_t * pcb;
    struct PCB_Node_t * next;
} PCB_Node_t;

// Define PCB linked list
typedef struct{
    PCB_Node_t * head;
    PCB_Node_t * tail;
} PCB_LinkedList;

// Public function prototypes
PCB_t * initPCB(int start, int end);
PCB_LinkedList * initPCBReadyQueue();
PCB_LinkedList * getPCBReadyQueue();
bool clearReadyQueue();
bool disposePCBReadyQueue();
bool enqueuePCB(PCB_t * pcb);
PCB_Node_t * dequeuePCB();

#endif
