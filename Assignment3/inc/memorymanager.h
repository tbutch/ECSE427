#ifndef MEM_MANAGER
#define MEM_MANAGER

#include "../inc/pcb.h"

// Public function prototypes
int launcher(FILE *p);
int countTotalPages(FILE * f, int * pageCount, int * lineCount);
bool loadPage(int pageNumber, PCB_t * pcb);
int findFrame();
int findVictim(PCB_t *pcb);
int updatePageTable(PCB_t * pcb, int pageNumber, int frameNumber, int victimFrame);
bool isPageLoaded(int pageNumber, PCB_t * pcb);
bool deleteFileInBackingStore(PCB_t * pcb);

// Constants
static const int MAX_BACKING_STORE_FILES = 99;
#define MAX_BACKING_STORE_FILES_CT 3
#define NO_VICTIM -1
static const char TXT_EXTENSION[] = ".txt";

#endif