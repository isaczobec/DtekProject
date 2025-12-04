#pragma once
#include "types.h"

// check which platform we are compiling to
#if defined(__DTEK__)
#define PLATFROM 1

#elif defined(__WINDOWS__)
#define PLATFROM 2

#else
#error Unknown platform, please define either '__WINDOWS__' or '__DTEK__'
#endif

// define more readable macros for windows and dtek
#define DTEK 1
#define WINDOWS 2

typedef struct {
    uint16 switches;
    uint8 pressedButton;
} InputState;

/* Function to initialize platform specefic functions.*/
void init();

/* Function to run to get input and poll events.
Returns 1 if the program should exit, else 0.*/
int poll();

InputState getInputState();