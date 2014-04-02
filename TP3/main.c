#include "msp430fg4618.h"
#include "lcd.h"

unsigned int cpt;

int main (void)
{
  WDTCTL = WDTPW + WDTHOLD; // stop watchdog timer
  
  lcd_init(); // Init Driver
  
  P5DIR |= 0x02; // Set P5.1 to output direction

  cpt = 0;
  for(;;) {
    volatile unsigned int i;
    for(i=0;i<0xFFF0;i++);
    
    lcd_display_number(cpt);
    cpt ++;
    
    P5OUT ^= 0x02; // toggle P5.1 (LED4)
  }
}