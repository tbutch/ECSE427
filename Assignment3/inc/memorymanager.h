#ifndef MEM_MANAGER
#define MEM_MANAGER

#include "../inc/pcb.h"

// Public function prototypes
int launcher(FILE *p);
int countTotalPages(FILE *f);
void loadPage(int pageNumber, FILE *f, int frameNumber);
int findFrame();
int findVictim(PCB_t *p);
int updatePageTable(PCB_t *p, int pageNumber, int frameNumber, int victimFrame);

#endif