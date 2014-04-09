#include "msp430fg4618.h"
#include "lcd.h"
#include "timer.h"
#include "button.h"

unsigned char is_paused;
unsigned int cpt;

// This will get executed 100 times per second
#pragma vector=TIMERA0_VECTOR
__interrupt void mon_traitement_interruption_timer(void)
{
  lcd_display_number(cpt);
  if (is_paused == 0)
    cpt ++;
}

// This will get executed when the push button is pressed
#pragma vector=PORT1_VECTOR
__interrupt void traitement_pression_bouton(void)
{
  if ((P1IFG & (1 << 1)) == (1 << 1))
    cpt = 0;
  else
    is_paused = 1 - is_paused;
  
  P1IFG = 0x0;
}

int main (void)
{
  WDTCTL = WDTPW + WDTHOLD; // stop watchdog timer
  
  __enable_interrupt();

  lcd_init(); // Init Driver
  init_push_button();
  
  is_paused = 0;
  cpt = 0;
  init_timer();
  
  for(;;);
}
