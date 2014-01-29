#include "msp430.h"

void testRelease()
{
  WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
  P5DIR = 0x02; // Configure P5.1
  P1DIR = P1DIR & 0xFC; // Configure push buttons as input
  for (;;) 
  {
    // While button is not pressed
    while ( (P1IN & 0x01) != 0x00 );
    // While button has not been released
    while ( (P1IN & 0x01) == 0x00 );
    
    // Toggle LED
    P5OUT = P5OUT ^ 0x02;
    
    volatile unsigned int i = 0;
    for (i=0 ; i < 10000 ; i++);
  }
}

int main(void)
{
  
  testRelease();
  
  WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
  P5DIR = 0x02; // Configure P5.1
  P1DIR = P1DIR & 0xFC; // Configure push buttons as input
  P3DIR = 0x20; //P3DIR & 0x20; // Configure buzzer as output
  for (;;) 
  {
    volatile unsigned int i = 0;
    
    // While button is not pressed
    while ( (P1IN & 0x01) != 0x00 )
    {
      // Reset LED
      P5OUT = 0x00;
      // Oscillate the buzzer
      P3OUT = P3OUT ^ 0x20;
      for (i=0 ; i < 400 ; i++);
      // wait for button to be pushed
    }
    
    // When button is pressed, turn on LED
    P5OUT = 0x02;
    
    // And oscillate buzzer faster
    P3OUT = P3OUT ^ 0x20;
    for (i=0 ; i < 40 ; i++);
  }
}