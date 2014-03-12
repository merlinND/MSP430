#include <stdint.h>

#include "msp430fg4618.h"
#include "lcd.h"

uint16_t alea()
{
  static uint16_t n = 1;
  
  n = n * 3 + 5;
  return n;
}

int main (void)
{
  WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
  P1DIR = P1DIR & 0xFC; // Configure push buttons as input
  lcd_init();
  
  unsigned int a = 30000;
  unsigned int b = 40000;
  unsigned long c = (unsigned long)a * b;
  
  lcd_display_number(c);
  for (;;) {
    // While button is not pressed
    while ( (P1IN & 0x01) != 0x00 );
    // While button has not been released
    while ( (P1IN & 0x01) == 0x00 ) { alea(); }
    
    unsigned int nb = alea();
    lcd_display_number(nb);
  }
}