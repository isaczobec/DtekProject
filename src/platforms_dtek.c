#if defined(__DTEK__)
#include "platforms.h"
#include "io/io.h" 
#include "game/game.h"

// pointer to the switches
volatile int* const SWITCH_POINTER = (volatile int*)0x04000010;

extern void enable_interrupt(void);

// returns the bitmask of the switches.
int get_sw() {
  return (*SWITCH_POINTER) & (0x3FF);
}

void handle_switch_interrupt() {
    uint8 position_switch_mask = (uint8)get_sw() & 0x3F; // the switch positions corresponding to changing the position of the player
    setSelectedSquare(position_switch_mask);
}

void handle_interrupt(unsigned cause) 
{
    
    print("interrupt recieved!\n");
    if (cause == 17) {
        print("switch interrupt\n");
        *(SWITCH_POINTER + 3) &= (~1);
        *(SWITCH_POINTER + 3) &= (~(1<<1));
        *(SWITCH_POINTER + 3) &= (~(1<<2));
        *(SWITCH_POINTER + 3) &= (~(1<<3));
        *(SWITCH_POINTER + 3) &= (~(1<<4));
        *(SWITCH_POINTER + 3) &= (~(1<<5));

        handle_switch_interrupt();
    } 
    else if (cause == 16) {
        print("timer interrupt\n");
    }

    return;
}
void init() {
    print("enablind interrupts..");
    enable_interrupt();

    // make the switches generate IRQs
    *(SWITCH_POINTER + 2) = *(SWITCH_POINTER + 2) | 1;
    *(SWITCH_POINTER + 2) = *(SWITCH_POINTER + 2) | 1<<1;
    *(SWITCH_POINTER + 2) = *(SWITCH_POINTER + 2) | 1<<2;
    *(SWITCH_POINTER + 2) = *(SWITCH_POINTER + 2) | 1<<3;
    *(SWITCH_POINTER + 2) = *(SWITCH_POINTER + 2) | 1<<4;
    *(SWITCH_POINTER + 2) = *(SWITCH_POINTER + 2) | 1<<5;
}



#endif