#include "msp430fg4618.h"

void init_push_button()
{
 // Choose GPIO function for port 1
  P1SEL = 0x0;
  P1DIR = P1DIR | 0 << 1; // IN direction
  P1DIR = P1DIR | 0 << 0; // IN direction
  // Interrupts will be generated on
  // 0 -> 1 transitions (button down)
  P1IES = P1IES | 0 << 1;
  P1IES = P1IES | 0 << 0;
  // Enable interruption for port P1.1 and P1.0
  P1IE = P1IE | 1 << 1;
  P1IE = P1IE | 1 << 0;
}