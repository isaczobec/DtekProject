#if defined(__WINDOWS__)
#include "io.h"
#include <stdio.h>

void print(char* string) {
    printf("%s",string);
}

void print_dec(uint32 x) {
    printf("%d",x);
}
void print_hex32(uint32 x) {
    printf("%X",x);
}

#endif