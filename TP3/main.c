#include "msp430fg4618.h"
#include "lcd.h"

unsigned int cpt;

int main (void)
{
  WDTCTL = WDTPW + WDTHOLD; // stop watchdog timer

  lcd_init(); // Init Driver

  P5DIR |= 0x02; // Set P5.1 to output direction
	P1DIR = P1DIR & 0xFC; // Configure push buttons as input

  cpt = 0;
  for(;;) {
    // While button is not pressed
    while ( (P1IN & 0x01) != 0x00 );
    // While button has not been released
    while ( (P1IN & 0x01) == 0x00 );

    lcd_display_number(cpt);
    cpt ++;

    P5OUT ^= 0x02; // toggle P5.1 (LED4)
  }
}
