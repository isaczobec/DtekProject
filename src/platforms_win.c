#if defined(__WINDOWS__)
#include "platforms.h"
#include "io/io.h"

void init() {
    
}
#endif

#if defined(__WINDOWS__)
// simple emulation of the hardware switch mask for Windows builds
static volatile int _win_switch_mask = 0;

int get_sw() {
	return _win_switch_mask & 0x3F;
}

void set_win_switch_mask(int mask) {
	_win_switch_mask = mask & 0x3F;
}

void press_win_button(void) {
	// nothing stored here; main will call confirmSelectedSquare() directly
}
#endif