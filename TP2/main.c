#include "msp430fg4618.h"
#include "lcd.h"

unsigned int alea()
{
  static unsigned int n = 1;
  
  n = n * 3 + 5;
  return n;
}

int main (void)
{
  WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
  P1DIR = P1DIR & 0xFC; // Configure push buttons as input
  lcd_init();
  lcd_display_seven_digits(1, 2, 3, 4, 5, 6, 7);
  for (;;) {
    // While button is not pressed
    while ( (P1IN & 0x01) != 0x00 );
    // While button has not been released
    while ( (P1IN & 0x01) == 0x00 ) { alea(); }
    
    unsigned int nb = alea();
    lcd_display_number(nb);
  }
}