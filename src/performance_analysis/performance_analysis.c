#include "performance_analysis.h"

uint32 mcycle;
uint32 mcycleh;
uint32 minstret;
uint32 minstreth;
uint32 mhpmcounter3;
uint32 mhpmcounter3h;
uint32 mhpmcounter4;
uint32 mhpmcounter4h;
uint32 mhpmcounter5;
uint32 mhpmcounter5h;
uint32 mhpmcounter6;
uint32 mhpmcounter6h;
uint32 mhpmcounter7;
uint32 mhpmcounter7h;
uint32 mhpmcounter8;
uint32 mhpmcounter8h;
uint32 mhpmcounter9;
uint32 mhpmcounter9h;

// clears all control and status registers.
void clearCsrRegs() {
    #if defined(__DTEK__)
    asm volatile ("csrw mcycle, x0");
    asm volatile ("csrw mcycleh, x0");
    asm volatile ("csrw minstret, x0");
    asm volatile ("csrw minstreth, x0");
    asm volatile ("csrw mhpmcounter3, x0");
    asm volatile ("csrw mhpmcounter3h, x0");
    asm volatile ("csrw mhpmcounter4, x0");
    asm volatile ("csrw mhpmcounter4h, x0");
    asm volatile ("csrw mhpmcounter5, x0");
    asm volatile ("csrw mhpmcounter5h, x0");
    asm volatile ("csrw mhpmcounter6, x0");
    asm volatile ("csrw mhpmcounter6h, x0");
    asm volatile ("csrw mhpmcounter7, x0");
    asm volatile ("csrw mhpmcounter7h, x0");
    asm volatile ("csrw mhpmcounter8, x0");
    asm volatile ("csrw mhpmcounter8h, x0");
    asm volatile ("csrw mhpmcounter9, x0");
    asm volatile ("csrw mhpmcounter9h, x0");
    #endif
}

void printCsrRegs() {
    #if defined(__DTEK__)
    uint32 timel;
    
    print("REGS:\n");

    asm volatile ("csrr %0, mcycle" : "=r"(timel)); print("mcycle: "); print_dec(timel); print("\n");
    asm volatile ("csrr %0, minstret" : "=r"(timel)); print("minstret: "); print_dec(timel); print("\n");
    asm volatile ("csrr %0, mhpmcounter3" : "=r"(timel)); print("mhpmcounter3: "); print_dec(timel); print("\n");
    asm volatile ("csrr %0, mhpmcounter4" : "=r"(timel)); print("mhpmcounter4: "); print_dec(timel); print("\n");
    asm volatile ("csrr %0, mhpmcounter5" : "=r"(timel)); print("mhpmcounter5: "); print_dec(timel); print("\n");
    asm volatile ("csrr %0, mhpmcounter6" : "=r"(timel)); print("mhpmcounter6: "); print_dec(timel); print("\n");
    asm volatile ("csrr %0, mhpmcounter7" : "=r"(timel)); print("mhpmcounter7: "); print_dec(timel); print("\n");
    asm volatile ("csrr %0, mhpmcounter8" : "=r"(timel)); print("mhpmcounter8: "); print_dec(timel); print("\n");
    asm volatile ("csrr %0, mhpmcounter9" : "=r"(timel)); print("mhpmcounter9: "); print_dec(timel); print("\n");
    #endif
}
void resetCsrCounterVars() {
    #if defined(__DTEK__)
    mcycle = 0;
    mcycleh = 0;
    minstret = 0;
    minstreth = 0;
    mhpmcounter3 = 0;
    mhpmcounter3h = 0;
    mhpmcounter4 = 0;
    mhpmcounter4h = 0;
    mhpmcounter5 = 0;
    mhpmcounter5h = 0;
    mhpmcounter6 = 0;
    mhpmcounter6h = 0;
    mhpmcounter7 = 0;
    mhpmcounter7h = 0;
    mhpmcounter8 = 0;
    mhpmcounter8h = 0;
    mhpmcounter9 = 0;
    mhpmcounter9h = 0;
    #endif
}

void incrementCsrCounterVars() {

    #if defined(__DTEK__)
    uint32 timel;
    
    
    asm volatile ("csrr %0, mcycle" : "=r"(timel));  
    mcycle += timel;
    asm volatile ("csrr %0, minstret" : "=r"(timel));  
    minstret += timel;
    asm volatile ("csrr %0, mhpmcounter3" : "=r"(timel));  
    mhpmcounter3 += timel;
    asm volatile ("csrr %0, mhpmcounter4" : "=r"(timel));  
    mhpmcounter4 += timel;
    asm volatile ("csrr %0, mhpmcounter5" : "=r"(timel));  
    mhpmcounter5 += timel;
    asm volatile ("csrr %0, mhpmcounter6" : "=r"(timel));  
    mhpmcounter6 += timel;
    asm volatile ("csrr %0, mhpmcounter7" : "=r"(timel));  
    mhpmcounter7 += timel;
    asm volatile ("csrr %0, mhpmcounter8" : "=r"(timel));  
    mhpmcounter8 += timel;
    asm volatile ("csrr %0, mhpmcounter9" : "=r"(timel));  
    mhpmcounter9 += timel;
    #endif
}

void printCsrCounterVars() {
    #if defined(__DTEK__)
    print("Regs Counted:\n");
    print("CV_mcycle: "); print_dec(mcycle); print("\n");
    print("CV_minstret: "); print_dec(minstret); print("\n");
    print("CV_mhpmcounter3: "); print_dec(mhpmcounter3); print("\n");
    print("CV_mhpmcounter4: "); print_dec(mhpmcounter4); print("\n");
    print("CV_mhpmcounter5: "); print_dec(mhpmcounter5); print("\n");
    print("CV_mhpmcounter6: "); print_dec(mhpmcounter6); print("\n");
    print("CV_mhpmcounter7: "); print_dec(mhpmcounter7); print("\n");
    print("CV_mhpmcounter8: "); print_dec(mhpmcounter8); print("\n");
    print("CV_mhpmcounter9: "); print_dec(mhpmcounter9); print("\n");
    #endif
}