#ifndef PCB
#define PCB

typedef struct{
    int PC;
    int max_lines;
    int pid;
    int pages_max;
    int pageTable[10];
    int PC_page;
    int PC_offset;
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
PCB_t * initPCB(int pid, int max_lines, int pages_max);
bool disposePCB(PCB_t * pcb);
PCB_LinkedList * initPCBReadyQueue();
PCB_LinkedList * getPCBReadyQueue();
bool clearReadyQueue();
bool disposePCBReadyQueue();
bool enqueuePCB(PCB_t * pcb);
PCB_t * dequeuePCB();
int getPCBQueueLength();

#endif
