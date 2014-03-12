#include <stdint.h>

#include "msp430fg4618.h"
#include "lcd.h"

uint16_t alea()
{
  static uint16_t n = 1;
  
  volatile uint16_t m = 13;
  
  n = n * m + 5;
  return n;
}

uint16_t mul(uint16_t x, uint16_t y);

int main (void)
{
  WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
  
  lcd_init();
  uint16_t a = mul(42,0);
  for (;;) {
    lcd_display_number(a);
  }
  /*
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
  }*/
}