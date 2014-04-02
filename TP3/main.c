#include "msp430fg4618.h"
#include "lcd.h"
#include "timer.h"

unsigned int cpt;

// This will get executed 100 times per second
#pragma vector=TIMERA0_VECTOR
__interrupt void mon_traitement_interruption_timer(void)
{
	lcd_display_number(cpt);
	cpt ++;
}

int main (void)
{
  WDTCTL = WDTPW + WDTHOLD; // stop watchdog timer
  
  __enable_interrupt();

  lcd_init(); // Init Driver

  P5DIR |= 0x02; // Set P5.1 to output direction
	P1DIR = P1DIR & 0xFC; // Configure push buttons as input

  cpt = 0;
  
  // While button is not pressed
  while ( (P1IN & 0x01) != 0x00 );
  // While button has not been released
  while ( (P1IN & 0x01) == 0x00 );
  init_timer();
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
