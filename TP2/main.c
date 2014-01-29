#include "msp430fg4618.h"
#include "lcd.h"

int main (void)
{
	WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
	lcd_init();
	for (;;); // endless loop
}