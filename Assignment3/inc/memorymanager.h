#ifndef MEM_MANAGER
#define MEM_MANAGER

#include "../inc/pcb.h"

// Public function prototypes
int launcher(FILE *p);
int countTotalPages(FILE * f, int * pageCount, int * lineCount);
void loadPage(int pageNumber, FILE *f, int frameNumber);
int findFrame();
int findVictim(PCB_t *pcb);
int updatePageTable(PCB_t * pcb, int pageNumber, int frameNumber, int victimFrame);

// Constants
static const int MAX_BACKING_STORE_FILES = 99;
#define MAX_BACKING_STORE_FILES_CT 3
#define NO_VICTIM -1
static const char TXT_EXTENSION[] = ".txt";

#endif