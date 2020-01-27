#ifndef NODE
#define NODE

// Generic linked list struct, with void pointer used
// as payload.
typedef void * pVoid;
typedef struct node_t{
    pVoid val;
    struct node_t * next;
} node;

// Function prototypes
node * createNode();
node * createNodeWithVal(pVoid val);
void disposeNode(node * node);
void addToEnd(node * head, node * nextNode);
void removeLast(node * head);
int getLength(node * head);
char * convertListToString(node * head);

#endif