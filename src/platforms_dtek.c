#if defined(__DTEK__)
#include "platforms.h"
#include "io/io.h" 
#include "game/game.h"

// pointer to the switches
volatile int* const SWITCH_POINTER = (volatile int*)0x04000010;
volatile int* const BUTTON_POINTER = (volatile int*)0x040000D0;

extern void enable_interrupt(void);

InputState currentState;


// returns the bitmask of the switches.
int get_sw() {
  return (*SWITCH_POINTER) & (0x3FF);
}

void handle_switch_interrupt() {

    currentState.switches = (uint8)get_sw();

    uint8 position_switch_mask = (uint8)get_sw() & 0x3F; // the switch positions corresponding to changing the position of the player
    setSelectedSquare(position_switch_mask);
}

void handle_button_interrupt() {
    // print("button interrupt\n");
    int buttonPressed = (*BUTTON_POINTER) & 1;
    if (buttonPressed) {
        currentState.pressedButton = 1;
        confirmSelectedSquare();
    }
}

void handle_interrupt(unsigned cause) 
{
    
    // print("interrupt recieved!\n");
    if (cause == 17) {
        // print("switch interrupt\n");
        *(SWITCH_POINTER + 3) &= (~1);
        *(SWITCH_POINTER + 3) &= (~(1<<1));
        *(SWITCH_POINTER + 3) &= (~(1<<2));
        *(SWITCH_POINTER + 3) &= (~(1<<3));
        *(SWITCH_POINTER + 3) &= (~(1<<4));
        *(SWITCH_POINTER + 3) &= (~(1<<5));
        
        handle_switch_interrupt();
    } 
    else if (cause == 18) {
        *(BUTTON_POINTER + 3) &= (~1);
        handle_button_interrupt();
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
    
    // make the button generate IRQs
    *(BUTTON_POINTER + 2) = *(BUTTON_POINTER + 2) | 1;
}

int poll() {
    asm volatile ("nop");
    return 0;
}

InputState getInputState() {
	InputState curr = currentState;
	currentState = (InputState){
		.switches = get_sw(),
		.pressedButton = 0
	};
	return curr;
}

#endif