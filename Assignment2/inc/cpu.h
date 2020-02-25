#ifndef CPU
#define CPU

typedef struct{
    int IP;
    char IR[1000];
    int quanta=2;
} CPU_t;

#endif
