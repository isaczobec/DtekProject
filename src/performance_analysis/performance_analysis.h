#pragma once
#include "../types.h"
#include "../io/io.h"


#define PRINT_REG(x) asm volatile \
("csrr %0, " #x : "=r"(timel));\
print(#x ": "); print_dec(timel); print("\n");

// clears all control and status registers.
void clearCsrRegs();
void printCsrRegs();

void resetCsrCounterVars();
void incrementCsrCounterVars();
void printCsrCounterVars();